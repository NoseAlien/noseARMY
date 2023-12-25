#include "ADXModelRenderer.h"
#include "ADXObject.h"
#include "ADXCommon.h"

void ADXModelRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	if (model_ != nullptr)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + texture_;
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		GetGameObject()->transform_.UpdateConstBuffer();

		// •`‰æƒRƒ}ƒ“ƒh
		model_->Draw(GetGameObject()->transform_.constBuffTransform_.Get());
	}
}