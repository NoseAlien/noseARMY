#include "ADXTextRenderer.h"
#include "ADXObject.h"

void ADXTextRenderer::AddFonts(const std::vector<fontAndChar>& fontSet)
{
	for (auto& setItr : fontSet)
	{
		bool hitted = false;
		for (auto& itr : fonts_)
		{
			if (setItr.character == itr.character)
			{
				itr = setItr;
				hitted = true;
				break;
			}
		}
		if (!hitted)
		{
			fonts_.push_back(setItr);
		}
	}
}

void ADXTextRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, [[maybe_unused]] ID3D12GraphicsCommandList* cmdList)
{
	uint32_t line = 0;
	ADXModel model = ADXModel::CreateRect();

	for (int i = 0;i < text_.size();i++)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + texture_;
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		GetGameObject()->transform_.UpdateConstBuffer();

		// 描画コマンド
		model.Draw(GetGameObject()->transform_);
	}
}
