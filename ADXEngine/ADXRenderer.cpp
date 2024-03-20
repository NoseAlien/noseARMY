#include "ADXRenderer.h"
#include "ADXCommon.h"
#include "ADXObject.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

void ADXRenderer::Rendering()
{
	// nullptrチェック
	[[maybe_unused]] ID3D12Device* device = ADXCommon::GetInstance()->GetDevice();
	[[maybe_unused]] ID3D12GraphicsCommandList* cmdList = ADXObject::GetCmdList();
	assert(device);
	assert(cmdList);

	HRESULT result = S_FALSE;
	//定数バッファへデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = material_.ambient_;
	constMap1->diffuse = material_.diffuse_;
	constMap1->specular = material_.specular_;
	constMap1->alpha = material_.alpha_;
	constBuffB1_->Unmap(0, nullptr);

	//定数バッファビュー(CBV)の設定コマンド
	cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());

	UniqueRendering(device, cmdList);
}

void ADXRenderer::UniqueInitialize()
{
	InitializeConstBufferMaterial(&constBuffB1_);
}

void ADXRenderer::InitializeConstBufferMaterial(ID3D12Resource** constBuff)
{
	ID3D12Device* device = ADXCommon::GetInstance()->GetDevice();

	if (device != nullptr)
	{
		HRESULT result = S_FALSE;

		//ヒープ設定
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		//リソース設定
		D3D12_RESOURCE_DESC cbResourceDesc{};
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
			IID_PPV_ARGS(constBuff));
		assert(SUCCEEDED(result));
	}
}

void ADXRenderer::LoadShader(ID3DBlob** shaderBlob, LPCWSTR filePath, LPCSTR pEntryPoint)
{
	HRESULT result = S_FALSE;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	//シェーダーの読み込みとコンパイル
	result = D3DCompileFromFile(
		filePath,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", pEntryPoint,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		shaderBlob, &errorBlob);

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
}

D3D12_GRAPHICS_PIPELINE_STATE_DESC ADXRenderer::CreateDefaultPipelineDesc(ID3DBlob* vsBlob, ID3DBlob* psBlob, D3D12_INPUT_ELEMENT_DESC inputLayout[], uint32_t numElements)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC ret{};

	//シェーダーの設定
	ret.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	ret.VS.BytecodeLength = vsBlob->GetBufferSize();
	ret.PS.pShaderBytecode = psBlob->GetBufferPointer();
	ret.PS.BytecodeLength = psBlob->GetBufferSize();

	//サンプルマスクの設定
	ret.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	//ラスタライザの設定
	ret.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; //背面をカリング
	ret.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	ret.RasterizerState.DepthClipEnable = true;

	//デプスステンシルステートの設定
	ret.DepthStencilState.DepthEnable = true; //深度テストを行う
	ret.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL; //書き込み許可
	ret.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //同じか小さければ合格
	ret.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	ret.BlendState.AlphaToCoverageEnable = true; //アルファが低い所は深度を書き込まない

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = ret.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//頂点レイアウトの設定
	ret.InputLayout.pInputElementDescs = inputLayout;
	ret.InputLayout.NumElements = numElements;

	//図形の形状設定（三角形）
	ret.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	ret.NumRenderTargets = 1;
	ret.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	ret.SampleDesc.Count = 1;
	return ret;
}

void ADXRenderer::CreateGraphicsPipelineState(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pipelineDesc, ID3D12PipelineState** pipelineState)
{
	HRESULT result = S_FALSE;

	//パイプラインにルートシグネチャをセット
	pipelineDesc->pRootSignature = ADXObject::GetRootSignature();
	// パイプラインステートの生成
	result = ADXCommon::GetInstance()->GetDevice()->
		CreateGraphicsPipelineState(pipelineDesc, IID_PPV_ARGS(pipelineState));
	assert(SUCCEEDED(result));
}
