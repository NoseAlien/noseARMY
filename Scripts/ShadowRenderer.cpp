#include "ShadowRenderer.h"
#include "ADXObject.h"

Microsoft::WRL::ComPtr<ID3D12PipelineState> ShadowRenderer::S_pipelineStateStencil = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ShadowRenderer::S_pipelineStateDraw = nullptr;
Microsoft::WRL::ComPtr<ID3D12PipelineState> ShadowRenderer::S_pipelineStateAntiStencil = nullptr;

void ShadowRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	if (model_ != nullptr)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle();
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		GetGameObject()->transform_.UpdateConstBuffer();

		// 描画コマンド
		model_->Draw(GetGameObject()->transform_.constBuffTransform_.Get());
	}
}

void ShadowRenderer::UniqueInitialize()
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

	depthStencilDesc.DepthEnable = true; //深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //同じか小さければ合格
	depthStencilDesc.StencilEnable = false; //ステンシルテストを行わない
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = depthStencilDesc.FrontFace.StencilFailOp;
	depthStencilDesc.BackFace.StencilDepthFailOp = depthStencilDesc.FrontFace.StencilDepthFailOp;
	depthStencilDesc.BackFace.StencilPassOp = depthStencilDesc.FrontFace.StencilPassOp;
	depthStencilDesc.BackFace.StencilFunc = depthStencilDesc.FrontFace.StencilFunc;

	pipelineDesc.DepthStencilState = depthStencilDesc;
	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineStateStencil);


	depthStencilDesc.DepthEnable = true; //深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER; //大きければ合格
	depthStencilDesc.StencilEnable = true; //ステンシルテストを行う
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = 0x00;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_ZERO;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
	depthStencilDesc.BackFace.StencilFailOp = depthStencilDesc.FrontFace.StencilFailOp;
	depthStencilDesc.BackFace.StencilDepthFailOp = depthStencilDesc.FrontFace.StencilDepthFailOp;
	depthStencilDesc.BackFace.StencilPassOp = depthStencilDesc.FrontFace.StencilPassOp;
	depthStencilDesc.BackFace.StencilFunc = depthStencilDesc.FrontFace.StencilFunc;

	pipelineDesc.DepthStencilState = depthStencilDesc;
	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineStateDraw);


	depthStencilDesc.DepthEnable = true; //深度テストを行う
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; //書き込み不可
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; //同じか小さければ合格
	depthStencilDesc.StencilEnable = false; //ステンシルテストを行わない
	depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	depthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = depthStencilDesc.FrontFace.StencilFailOp;
	depthStencilDesc.BackFace.StencilDepthFailOp = depthStencilDesc.FrontFace.StencilDepthFailOp;
	depthStencilDesc.BackFace.StencilPassOp = depthStencilDesc.FrontFace.StencilPassOp;
	depthStencilDesc.BackFace.StencilFunc = depthStencilDesc.FrontFace.StencilFunc;

	pipelineDesc.DepthStencilState = depthStencilDesc;
	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineStateAntiStencil);
}