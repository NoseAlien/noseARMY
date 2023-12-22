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

uint32_t ADXTextRenderer::GetFontTex(const char& character)
{
	for (auto& itr : fonts_)
	{
		if (itr.character == character)
		{
			return itr.font;
		}
	}
	return 0;
}

void ADXTextRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	GetGameObject()->transform_.UpdateConstBuffer();

	fontWtfs_.clear();
	for (int i = 0;i < text_.size();i++)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + GetFontTex(text_[i]);
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		fontWtfs_.push_back(ADXWorldTransform());
		fontWtfs_.back().Initialize(GetGameObject());
		fontWtfs_.back().parent_ = &GetGameObject()->transform_;
		fontWtfs_.back().rectTransform_ = GetGameObject()->transform_.rectTransform_;
		fontWtfs_.back().localPosition_.x_ = fontSize_ * fontAspect_ * i * 2;
		fontWtfs_.back().UpdateMatrix();
		fontWtfs_.back().UpdateConstBuffer();

		// 描画コマンド
		model_.Draw(fontWtfs_.back().constBuffTransform_.Get());
	}
}