#include "ADXTextRenderer.h"
#include "ADXObject.h"

void ADXTextRenderer::AddFonts(const std::vector<fontAndChar>& fontSet)
{
	//既に設定されている文字は上書き、そうでなければ新規作成
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
	//検索してヒットしたらそれを返す
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

	float fontWidth = fontSize_ * fontAspect_;

	fontWtfs_.clear();
	for (int i = 0;i < text_.size();i++)
	{
		D3D12_GPU_DESCRIPTOR_HANDLE S_gpuDescHandleSRV;
		S_gpuDescHandleSRV.ptr = ADXObject::GetGpuStartHandle() + GetFontTex(text_[i]);
		cmdList->SetGraphicsRootDescriptorTable(1, S_gpuDescHandleSRV);

		//文字ごとにトランスフォームを生成
		fontWtfs_.push_back(ADXWorldTransform());
		fontWtfs_.back().Initialize(GetGameObject());
		fontWtfs_.back().parent_ = &GetGameObject()->transform_;
		fontWtfs_.back().rectTransform_ = GetGameObject()->transform_.rectTransform_;
		//文字をずらす
		switch (anchor_)
		{
		case upperLeft:
			fontWtfs_.back().localPosition_ = { fontWidth * (i * 2 + 1),-fontSize_,0 };
			break;
		case upperCenter:
			fontWtfs_.back().localPosition_ = { fontWidth * (i * 2 + 1 - (float)text_.size()),-fontSize_,0 };
			break;
		case upperRight:
			fontWtfs_.back().localPosition_ = { fontWidth * ((i - (float)text_.size()) * 2 + 1),-fontSize_,0 };
			break;
		case middleLeft:
			fontWtfs_.back().localPosition_ = { fontWidth * (i * 2 + 1),0,0 };
			break;
		case middleCenter:
			fontWtfs_.back().localPosition_ = { fontWidth * (i * 2 + 1 - (float)text_.size()),0,0 };
			break;
		case middleRight:
			fontWtfs_.back().localPosition_ = { fontWidth * ((i - (float)text_.size()) * 2 + 1),0,0 };
			break;
		case lowerLeft:
			fontWtfs_.back().localPosition_ = { fontWidth * (i * 2 + 1),fontSize_,0 };
			break;
		case lowerCenter:
			fontWtfs_.back().localPosition_ = { fontWidth * (i * 2 + 1 - (float)text_.size()),fontSize_,0 };
			break;
		case lowerRight:
			fontWtfs_.back().localPosition_ = { fontWidth * ((i - (float)text_.size()) * 2 + 1),fontSize_,0 };
			break;
		}
		fontWtfs_.back().UpdateMatrix();
		fontWtfs_.back().UpdateConstBuffer();

		// 描画コマンド
		model_.Draw(fontWtfs_.back().constBuffTransform_.Get());
	}
}