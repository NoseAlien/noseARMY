#pragma once

#include "ADXModel.h"
#include "ADXRenderer.h"

class ShadowRenderer : public ADXRenderer
{
public:
	ADXModel* model_ = nullptr;
	uint32_t texture_ = 0;

private:
	ADXMaterial stencilMat{};
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffStencil_ = nullptr;

private:
	void RendererInitialize() final;
	//描画処理
	void UniqueRendering(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

private:
	// パイプラインステートオブジェクト（ステンシル用）
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineStateStencil;
	// パイプラインステートオブジェクト（影描画用）
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineStateDraw;
	// パイプラインステートオブジェクト（ステンシル後始末用）
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> S_pipelineStateAntiStencil;

public:
	//クラスを初期化
	static void StaticInitialize();
};