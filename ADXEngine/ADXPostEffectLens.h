#pragma once
#include "ADXObject.h"

class ADXPostEffectLens : public ADXComponent
{
private:
	ADXModel rect_{};
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV_ = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV_ = nullptr;

	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// グラフィックスパイプライン
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_;
public:
	ADXPostEffectLens();
	void UniqueInitialize();
	void CreateGraphicsPipelineState();

private:
	void OnPreRender();
	void OnWillRenderObject();
};