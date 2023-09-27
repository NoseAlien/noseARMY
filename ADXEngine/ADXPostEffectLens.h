#pragma once
#include "ADXObject.h"

class ADXPostEffectLens : public ADXComponent
{
private:
	ADXModel rect{};
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
public:
	ADXPostEffectLens();
	void UniqueInitialize();
	void CreateGraphicsPipelineState();

private:
	void OnPreRender();
	void OnWillRenderObject();
};