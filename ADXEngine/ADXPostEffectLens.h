#pragma once
#include "ADXObject.h"

class ADXPostEffectLens : public ADXComponent
{
private:
	ADXModel rect{};
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuff = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapRTV = nullptr;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeapDSV = nullptr;

	// ���[�g�V�O�l�`��
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// �O���t�B�b�N�X�p�C�v���C��
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
public:
	ADXPostEffectLens();
	void UniqueInitialize();
	void CreateGraphicsPipelineState();

private:
	void OnPreRender();
	void OnWillRenderObject();
};