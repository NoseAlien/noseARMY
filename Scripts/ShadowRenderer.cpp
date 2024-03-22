#include "ShadowRenderer.h"
#include "ADXObject.h"

const UINT8 shadowStencil = 1;

Microsoft::WRL::ComPtr<ID3D12PipelineState> ShadowRenderer::S_pipelineStateStencil = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ShadowRenderer::S_pipelineStateDraw = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ShadowRenderer::S_pipelineStateAntiStencil = nullptr;

void ShadowRenderer::RendererInitialize()
{
	InitializeConstBufferMaterial(&constBuffStencil_);
}

void ShadowRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	if (model_ != nullptr)
	{
		GetGameObject()->transform_.UpdateConstBuffer();

		//モデルに貼り付ける画像を設定
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + texture_;
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);

		HRESULT result = S_FALSE;
		//定数バッファへデータ転送
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffStencil_->Map(0, nullptr, (void**)&constMap1);
		constMap1->alpha = 0;
		constBuffStencil_->Unmap(0, nullptr);

		//定数バッファへデータ転送
		result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material_.ambient_;
		constMap1->diffuse = material_.diffuse_;
		constMap1->specular = material_.specular_;
		constMap1->alpha = material_.alpha_;
		constBuffB1_->Unmap(0, nullptr);

		//定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(2, constBuffStencil_->GetGPUVirtualAddress());
		// パイプラインステートの設定コマンド
		cmdList->SetPipelineState(S_pipelineStateStencil.Get());
		// 描画コマンド
		model_->Draw(GetGameObject()->transform_.constBuffTransform_.Get());

		//定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());
		// パイプラインステートの設定コマンド
		cmdList->SetPipelineState(S_pipelineStateDraw.Get());
		// 描画コマンド
		model_->Draw(GetGameObject()->transform_.constBuffTransform_.Get());

		//定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(2, constBuffStencil_->GetGPUVirtualAddress());
		//// パイプラインステートの設定コマンド
		//cmdList->SetPipelineState(S_pipelineStateAntiStencil.Get());
		//// 描画コマンド
		//model_->Draw(GetGameObject()->transform_.constBuffTransform_.Get());
	}
}

void ShadowRenderer::StaticInitialize()
{
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

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト

	LoadShader(&vsBlob, L"Resources/shader/OBJVertexShader.hlsl", "vs_5_0");
	LoadShader(&psBlob, L"Resources/shader/OBJPixelShader.hlsl", "ps_5_0");

	//グラフィックスパイプラインの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc =
		CreateDefaultPipelineDesc(vsBlob.Get(), psBlob.Get(), inputLayout, _countof(inputLayout));

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	//影描画前に貼るステンシル
	depthStencilDesc.DepthEnable = true; //深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //同じか小さければ合格
	depthStencilDesc.StencilEnable = true; //ステンシルテストを行う
	depthStencilDesc.StencilReadMask = shadowStencil;
	depthStencilDesc.StencilWriteMask = shadowStencil;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = depthStencilDesc.FrontFace.StencilFailOp;
	depthStencilDesc.BackFace.StencilDepthFailOp = depthStencilDesc.FrontFace.StencilDepthFailOp;
	depthStencilDesc.BackFace.StencilPassOp = depthStencilDesc.FrontFace.StencilPassOp;
	depthStencilDesc.BackFace.StencilFunc = depthStencilDesc.FrontFace.StencilFunc;

	pipelineDesc.DepthStencilState = depthStencilDesc;
	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineStateStencil);

	//影本体の描画
	depthStencilDesc.DepthEnable = true; //深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER; //大きければ合格
	depthStencilDesc.StencilEnable = true; //ステンシルテストを行う
	depthStencilDesc.StencilReadMask = shadowStencil;
	depthStencilDesc.StencilWriteMask = shadowStencil;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
	depthStencilDesc.BackFace.StencilFailOp = depthStencilDesc.FrontFace.StencilFailOp;
	depthStencilDesc.BackFace.StencilDepthFailOp = depthStencilDesc.FrontFace.StencilDepthFailOp;
	depthStencilDesc.BackFace.StencilPassOp = depthStencilDesc.FrontFace.StencilPassOp;
	depthStencilDesc.BackFace.StencilFunc = depthStencilDesc.FrontFace.StencilFunc;

	pipelineDesc.DepthStencilState = depthStencilDesc;
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT; //前面をカリング
	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineStateDraw);

	//ステンシルを元に戻す描画
	depthStencilDesc.DepthEnable = false; //深度テストを行わない
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; //必ず合格
	depthStencilDesc.StencilEnable = true; //ステンシルテストを行う
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = ADXObject::clearStencil;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = depthStencilDesc.FrontFace.StencilFailOp;
	depthStencilDesc.BackFace.StencilDepthFailOp = depthStencilDesc.FrontFace.StencilDepthFailOp;
	depthStencilDesc.BackFace.StencilPassOp = depthStencilDesc.FrontFace.StencilPassOp;
	depthStencilDesc.BackFace.StencilFunc = depthStencilDesc.FrontFace.StencilFunc;

	pipelineDesc.DepthStencilState = depthStencilDesc;
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; //カリングなし
	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineStateAntiStencil);
}