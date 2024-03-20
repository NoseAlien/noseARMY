#include "ADXModelRenderer.h"
#include "ADXObject.h"
#include "ADXCommon.h"

Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXModelRenderer::S_pipelineState = nullptr;

void ADXModelRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	if (model_ != nullptr)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + texture_;
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);

		// パイプラインステートの設定コマンド
		cmdList->SetPipelineState(S_pipelineState.Get());

		GetGameObject()->transform_.UpdateConstBuffer();

		// 描画コマンド
		model_->Draw(GetGameObject()->transform_.constBuffTransform_.Get());
	}
}

void ADXModelRenderer::StaticInitialize()
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

	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineState);
}
