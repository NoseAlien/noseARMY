#include "ADXModelRenderer.h"

void ADXModelRenderer::UniqueRendering()
{
	// nullptrチェック
	[[maybe_unused]] ID3D12Device* device = ADXCommon::GetCurrentInstance()->GetDevice();
	assert(device);
	assert(cmdList);

	if (useDefaultDraw_ && model_ != nullptr)
	{
		HRESULT result = S_FALSE;
		//定数バッファへデータ転送
		ConstBufferDataB1* constMap1 = nullptr;
		result = constBuffB1_->Map(0, nullptr, (void**)&constMap1);
		constMap1->ambient = material_.ambient_;
		constMap1->diffuse = material_.diffuse_;
		constMap1->specular = material_.specular_;
		constMap1->alpha = material_.alpha_;
		constBuffB1_->Unmap(0, nullptr);

		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = S_GpuStartHandle + texture_;
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		//定数バッファビュー(CBV)の設定コマンド
		cmdList->SetGraphicsRootConstantBufferView(2, constBuffB1_->GetGPUVirtualAddress());

		transform_.UpdateConstBuffer();

		// 描画コマンド
		model_->Draw(transform_);
	}
}