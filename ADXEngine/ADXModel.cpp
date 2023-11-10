#include "ADXModel.h"
#include "ADXCommon.h"
#include <math.h>

#include <fstream>
#include <sstream>

using namespace DirectX;

ADXModel::ADXModel()
{

}

ADXModel ADXModel::LoadADXModel(const std::string& filePath)
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
		std::string key_;
		std::getline(line_stream, key_, ' ');

		//先頭文字列がvなら頂点座標
		if (key_ == "v")
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
		if (key_ == "vt")
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
		if (key_ == "vn")
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
		if (key_ == "f")
		{
			int32_t polyCount = 0;
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
				model.vertices_.emplace_back(vertex);
				//インデックスデータの追加
				unsigned short nowIndex = (unsigned short)model.vertices_.size() - 1;
				model.indices_.emplace_back(nowIndex);
				if (polyCount >= 3)
				{
					model.indices_.emplace_back(firstIndex);
					model.indices_.emplace_back(lastIndex);
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
	for (int32_t i = 0; i < indices_.size() / 3; i++)
	{//三角形一つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short index0 = indices_[i * 3];
		unsigned short index1 = indices_[i * 3 + 1];
		unsigned short index2 = indices_[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices_[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices_[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices_[index2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算（ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//正規化
		normal = XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		XMStoreFloat3(&vertices_[index0].normal, normal);
		XMStoreFloat3(&vertices_[index1].normal, normal);
		XMStoreFloat3(&vertices_[index2].normal, normal);
	}
}

///<summary>
///頂点バッファビューを作成する処理を一括で行う
///</summary>
void ADXModel::CreateVertexBufferView()
{
	HRESULT result;

	//頂点データ全体のサイズ = 一つの頂点データのサイズ * 頂点データの要素数
	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());
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
	result = ADXCommon::GetCurrentInstance()->GetDevice()
		->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&vertBuff_));
	assert(SUCCEEDED(result));

	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対し座標をコピー
	for (int32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i];
	}
	//つながりを削除
	vertBuff_->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);
}

///<summary>
///インデックスバッファビューを作成する処理を一括で行う
///</summary>
void ADXModel::CreateIndexBufferView()
{
	HRESULT result;

	//インデックスデータ全体のサイズ
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());
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
	result = ADXCommon::GetCurrentInstance()->GetDevice()
		->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff_));

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対しインデックスをコピー
	for (int32_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];
	}
	//つながりを削除
	indexBuff_->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
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

void ADXModel::Draw(ID3D12GraphicsCommandList* commandList, const ADXWorldTransform& wtf_)
{
	// モデルデータを更新
	Update();

	// 頂点バッファビューの設定コマンド
	commandList->IASetVertexBuffers(0, 1, &vbView_);

	// インデックスバッファビューの設定コマンド
	commandList->IASetIndexBuffer(&ibView_);

	//定数バッファビュー(CBV)の設定コマンド
	commandList->SetGraphicsRootConstantBufferView(0, wtf_.constBuffTransform_->GetGPUVirtualAddress());

	// 描画コマンド
	commandList->DrawIndexedInstanced((uint32_t)indices_.size(), 1, 0, 0, 0); // 全ての頂点を使って描画
}

void ADXModel::Update()
{
	//法線を自動設定
	SetNormal();

	HRESULT result;

	//頂点データ全体のサイズ = 一つの頂点データのサイズ * 頂点データの要素数
	uint32_t sizeVB = static_cast<uint32_t>(sizeof(vertices_[0]) * vertices_.size());

	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	Vertex* vertMap = nullptr;
	result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対し座標をコピー
	for (int32_t i = 0; i < vertices_.size(); i++)
	{
		vertMap[i] = vertices_[i];
	}
	//つながりを削除
	vertBuff_->Unmap(0, nullptr);

	//GPU仮想アドレス
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//頂点バッファのサイズ
	vbView_.SizeInBytes = sizeVB;
	//頂点一つ分のデータサイズ
	vbView_.StrideInBytes = sizeof(vertices_[0]);


	//インデックスデータ全体のサイズ
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices_.size());

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff_->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対しインデックスをコピー
	for (int32_t i = 0; i < indices_.size(); i++)
	{
		indexMap[i] = indices_[i];
	}
	//つながりを削除
	indexBuff_->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	ibView_.Format = DXGI_FORMAT_R16_UINT;
	ibView_.SizeInBytes = sizeIB;
}

ADXModel ADXModel::CreateRect()
{
	ADXModel rect_;
	rect_.vertices_ = {
	{{-1.0f,-1.0f,0.0f},{}, {0.0f,1.0f}},//左下
	{{-1.0f,1.0f,0.0f},{},{0.0f,0.0f}},//左上
	{{1.0f,-1.0f,0.0f},{},{1.0f,1.0f}},//右下
	{{1.0f,1.0f,0.0f},{},{1.0f,0.0f}},//右上
	};
	//インデックスデータ
	rect_.indices_ =
	{
		0,1,2,
		2,1,3,

		1,0,2,
		1,2,3,
	};
	rect_.Initialize();
	return rect_;
}
