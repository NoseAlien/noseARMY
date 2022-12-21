#include "ADXObject.h"
#include "ADXCommon.h"
#include <d3dcompiler.h>

ID3D12Device* ADXObject::device = nullptr;
UINT ADXObject::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ADXObject::cmdList = nullptr;
Microsoft::WRL::ComPtr<ID3D12RootSignature> ADXObject::rootSignature;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXObject::pipelineState;
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> ADXObject::descHeap;
Microsoft::WRL::ComPtr<ID3D12Resource> ADXObject::vertBuff;
Microsoft::WRL::ComPtr<ID3D12Resource> ADXObject::indexBuff;
Microsoft::WRL::ComPtr<ID3D12Resource> ADXObject::texbuff;
D3D12_CPU_DESCRIPTOR_HANDLE ADXObject::cpuDescHandleSRV;
D3D12_GPU_DESCRIPTOR_HANDLE ADXObject::gpuDescHandleSRV;
D3D12_CPU_DESCRIPTOR_HANDLE* ADXObject::dsvHandle = nullptr;
ADXMatrix4 ADXObject::matView = IdentityMatrix();
ADXMatrix4 ADXObject::matProjection = IdentityMatrix();
XMFLOAT3 ADXObject::eye = { 0, 0, -50.0f };
XMFLOAT3 ADXObject::target = { 0, 0, 0 };
XMFLOAT3 ADXObject::up = { 0, 1, 0 };
D3D12_VERTEX_BUFFER_VIEW ADXObject::vbView{};
D3D12_INDEX_BUFFER_VIEW ADXObject::ibView{};
std::vector<ADXObject*> ADXObject::allObjPtr{};
std::vector<ADXObject*> ADXObject::objs{};

ADXObject::ADXObject()
{

}

void ADXObject::StaticInitialize(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE* dsvHandle)
{
	// nullptrチェック
	assert(device);

	ADXObject::device = device;
	ADXObject::dsvHandle = dsvHandle;

	// パイプライン初期化
	InitializeGraphicsPipeline();
}

void ADXObject::Initialize()
{
	transform.Initialize();
	CreateConstBuffer();
}

std::vector<ADXObject*> ADXObject::GetAllObjs()
{
	return objs;
}

void ADXObject::CreateConstBuffer()
{
	if (device != nullptr)
	{
		HRESULT result = S_FALSE;

		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB0) + 0xff) & ~0xff;//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//定数バッファの生成
		result = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&transform.constBuffTransform));
		assert(SUCCEEDED(result));
		//定数バッファのマッピング
		result = transform.constBuffTransform->Map(0, nullptr, (void**)&transform.constMapTransform);//マッピング
		assert(SUCCEEDED(result));
		

		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(ConstBufferDataB1) + 0xff) & ~0xff;//256バイトアラインメント
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		//定数バッファの生成
		result = device->CreateCommittedResource(
			&cbHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffB1));
		assert(SUCCEEDED(result));
	}
}

void ADXObject::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	//頂点シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/OBJVertexShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vsBlob, &errorBlob);

	//上の読み込みでエラーが起きたら出力ウィンドウに内容を表示
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//ピクセルシェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shader/OBJPixelShader.hlsl",
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&psBlob, &errorBlob);

	//上の読み込みでエラーが起きたら出力ウィンドウに内容を表示
	if (FAILED(result))
	{
		//errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";

		OutputDebugStringA(error.c_str());
		assert(0);
	}

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//三次元座標
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//法線ベクトル
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//uv座標
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	//グラフィックスパイプラインの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	//シェーダーの設定
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//ラスタライザの設定
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; //背面をカリング
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	pipelineDesc.RasterizerState.DepthClipEnable = true;

	//デプスステンシルステートの設定
	pipelineDesc.DepthStencilState.DepthEnable = true; //深度テストを行う
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; //書き込み許可
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //同じか小さければ合格
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	pipelineDesc.BlendState.AlphaToCoverageEnable = true;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//ブレンドステート
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	//頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定（三角形）
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	pipelineDesc.NumRenderTargets = 1;
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	pipelineDesc.SampleDesc.Count = 1;

	//デスクリプタレンジの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;//一度の描画に使う画像が1枚なので
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;//テクスチャレジスタ0番
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメーターの設定
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	//定数バッファ0番
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//種類
	rootParams[0].Descriptor.ShaderRegister = 0;//定数バッファ番号
	rootParams[0].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える
	//テクスチャレジスタ0番
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//種類
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;//デスクリプタレンジ
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;//デスクリプタレンジ数
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える
	//定数バッファ1番
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//種類
	rootParams[2].Descriptor.ShaderRegister = 1;//定数バッファ番号
	rootParams[2].Descriptor.RegisterSpace = 0;//デフォルト値
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//全てのシェーダーから見える

	//テクスチャサンプラーの設定
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し(タイリング)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し(タイリング)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し(タイリング)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;//全てリニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダーからのみ使用可能

	//ルートシグネチャの設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;//ルートパラメーターの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);//ルートパラメーター数
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	//ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	//パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();
	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

void ADXObject::Update()
{
	HRESULT result = S_FALSE;

	for (int i = 0; i < colliders.size(); i++)
	{
		colliders[i].Update(this);
	}
	for (int i = 0; i < players.size(); i++)
	{
		players[i].Update(this);
	}

	allObjPtr.push_back(this);

	//定数バッファへデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material.ambient;
	constMap1->diffuse = material.diffuse;
	constMap1->specular = material.specular;
	constMap1->alpha = material.alpha;
	constBuffB1->Unmap(0, nullptr);
}

void ADXObject::StaticUpdate()
{
	objs = allObjPtr;
}

void ADXObject::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ADXObject::cmdList == nullptr);

	// コマンドリストをセット
	ADXObject::cmdList = cmdList;

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト
}

void ADXObject::StaticDraw(ID3D12DescriptorHeap* srvHeap)
{
	// プリミティブ形状の設定コマンド
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 三角形リスト

	//SRVヒープの設定コマンド
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//SRVヒープの先頭ハンドルを取得(SRVを指しているはず)
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
	//SRVヒープの先頭にあるSRVをルートパラメーター1番に設定
	cmdList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
	UINT64 GpuStartHandle = srvGpuHandle.ptr;

	// パイプラインステートとルートシグネチャの設定コマンド
	cmdList->SetPipelineState(pipelineState.Get());
	cmdList->SetGraphicsRootSignature(rootSignature.Get());

	int minLayer = 0;
	int maxLayer = 0;

	if (allObjPtr.size() > 0)
	{
		minLayer = allObjPtr[0]->renderLayer;
		maxLayer = allObjPtr[0]->renderLayer;
	}

	//RenderLayerの範囲を読み取る
	for (int i = 0; i < allObjPtr.size(); i++)
	{
		if (allObjPtr[i]->renderLayer < minLayer)
		{
			minLayer = allObjPtr[i]->renderLayer;
		}
		if (allObjPtr[i]->renderLayer > maxLayer)
		{
			maxLayer = allObjPtr[i]->renderLayer;
		}
	}

	for (int nowLayer = minLayer; nowLayer <= maxLayer; nowLayer++)
	{
		std::vector<ADXObject*> thisLayerObjPtr;

		//全ピクセルの深度バッファ値を最奥の1.0にする
		cmdList->ClearDepthStencilView(*dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		for (int i = 0; i < allObjPtr.size(); i++)
		{
			if (allObjPtr[i]->renderLayer  == nowLayer)
			{
				thisLayerObjPtr.push_back(allObjPtr[i]);
			}
		}

		//ためたオブジェクトを深度の深い順に取り出しながら描画
		for (int i = 0; thisLayerObjPtr.size() > 0; i++)
		{
			float dist = 0;
			int target = 0;
			for (int j = 0; j < thisLayerObjPtr.size(); j++)
			{
				thisLayerObjPtr[j]->transform.UpdateMatrix();
				float zDepth;
				if (thisLayerObjPtr[j]->transform.parent_ == nullptr)
				{
					zDepth = ADXMatrix4::transform(thisLayerObjPtr[j]->transform.translation_, matView).z;
				}
				else
				{
					zDepth = ADXMatrix4::transform(ADXMatrix4::transform(thisLayerObjPtr[j]->transform.translation_, thisLayerObjPtr[j]->transform.parent_->matWorld_), matView).z;
				}

				if (zDepth >= dist)
				{
					dist = zDepth;
					target = j;
				}
			}
			thisLayerObjPtr[target]->Draw(GpuStartHandle);
			thisLayerObjPtr.erase(thisLayerObjPtr.begin() + target);
		}
	}
}

void ADXObject::PostDraw()
{
	// コマンドリストを解除
	ADXObject::cmdList = nullptr;
	//ソート用配列を空にする
	allObjPtr.clear();
}

void ADXObject::Draw(UINT64 GpuStartHandle)
{
	// nullptrチェック
	assert(device);
	assert(cmdList);

	if (model != nullptr)
	{
		gpuDescHandleSRV.ptr = GpuStartHandle + texture.GetGHandle();
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);

		//定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1->GetGPUVirtualAddress());

		// 描画コマンド
		model->Draw(cmdList, transform);
	}
}