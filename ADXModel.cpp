#include "ADXModel.h"
#include <math.h>

#include <fstream>
#include <sstream>
#include <string>

ID3D12Device* ADXModel::device = nullptr;

ADXModel::ADXModel()
{

}

void ADXModel::StaticInitialize(ID3D12Device* setDevice)
{
	device = setDevice;
}

ADXModel ADXModel::LoadModel(const std::string filePath)
{
	ADXModel model;

	std::ifstream file;
	file.open("Resources/" + filePath);

	if (file.fail())
	{
		assert(0);
	}

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	std::vector<XMFLOAT2> texcoords;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream line_stream(line);
		std::string key;
		std::getline(line_stream, key, ' ');

		//先頭文字列がvなら頂点座標
		if (key == "v")
		{
			//XYZ座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//UV成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//XYZ成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン
		if (key == "f")
		{
			int polyCount = 0;
			unsigned short firstIndex{};
			unsigned short lastIndex{};
			//半角スペース区切りで行の続きを読み込む
			std::string index_string;
			while (std::getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexTexcoord;
				index_stream.seekg(1, std::ios_base::cur);
				index_stream >> indexNormal;
				//頂点データの追加
				Vertex vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				model.vertices.emplace_back(vertex);
				//インデックスデータの追加
				unsigned short nowIndex = (unsigned short)model.vertices.size() - 1;
				model.indices.emplace_back(nowIndex);
				if (polyCount >= 3)
				{
					model.indices.emplace_back(firstIndex);
					model.indices.emplace_back(lastIndex);
				}
				if (polyCount == 0)
				{
					firstIndex = nowIndex;
				}
				lastIndex = nowIndex;
				polyCount++;
			}
		}
	}
	file.close();

	model.Initialize();

	return model;
}

void ADXModel::SetNormal()
{
	for (int i = 0; i < indices.size() / 3; i++)
	{//三角形一つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices[i * 3];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}
}

///<summary>
///頂点バッファビューを作成する処理を一括で行う
///</summary>
void ADXModel::CreateVertexBufferView()
{
	HRESULT result;

	//頂点データ全体のサイズ = 一つの頂点データのサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//頂点バッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対し座標をコピー
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];
	}
	//つながりを削除
	vertBuff->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);
}

///<summary>
///インデックスバッファビューを作成する処理を一括で行う
///</summary>
void ADXModel::CreateIndexBufferView()
{
	HRESULT result;

	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
	//インデックスバッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//インデックスバッファの生成
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対しインデックスをコピー
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];
	}
	//つながりを削除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void ADXModel::Initialize()
{
	//法線を自動設定
	SetNormal();

	//頂点バッファビュー作成
	CreateVertexBufferView();

	//インデックスバッファビュー作成
	CreateIndexBufferView();
}

void ADXModel::Draw(ID3D12GraphicsCommandList* commandList, ADXWorldTransform wtf_)
{
	// 頂点バッファビューの設定コマンド
	commandList->IASetVertexBuffers(0, 1, &vbView);

	// インデックスバッファビューの設定コマンド
	commandList->IASetIndexBuffer(&ibView);

	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, wtf_.constBuffTransform->GetGPUVirtualAddress());

	// 描画コマンド
	commandList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0); // 全ての頂点を使って描画	
}