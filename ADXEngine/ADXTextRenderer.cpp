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
	for (int i = 0;i < text_.size();i++)
	{
		ADXModel fontModel = model_;

		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + GetFontTex(text_[i]);
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		for (auto& itr : fontModel.vertices_)
		{
			itr.pos.x += fontSize_ * fontAspect_ * i;
		}

		GetGameObject()->transform_.UpdateConstBuffer();

		// 描画コマンド
		fontModel.Draw(GetGameObject()->transform_.constBuffTransform_.Get());
	}
}