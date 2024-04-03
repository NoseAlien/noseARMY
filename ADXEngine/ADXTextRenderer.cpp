#include "ADXTextRenderer.h"
#include "ADXObject.h"

Microsoft::WRL::ComPtr<ID3D12PipelineState> ADXTextRenderer::S_pipelineState = nullptr;
std::vector<ADXTextRenderer::font> ADXTextRenderer::S_fonts{};

uint32_t ADXTextRenderer::GetFontTex(const char& character)
{
	if (font_ != nullptr)
	{
		//検索してヒットしたらそれを返す
		for (auto& itr : font_->fontDataCells_)
		{
			if (itr.character == character)
			{
				return itr.font;
			}
		}
	}
	return 0;
}

void ADXTextRenderer::UniqueRendering([[maybe_unused]] ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{

	// パイプラインステートの設定コマンド
	cmdList->SetPipelineState(S_pipelineState.Get());

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
		//文字の拡大率を適用
		fontWtfs_.back().localScale_ = { fontExtend_,fontExtend_,fontExtend_ };
		fontWtfs_.back().UpdateMatrix();
		fontWtfs_.back().UpdateConstBuffer();

		// 描画コマンド
		model_->Draw(fontWtfs_.back().constBuffTransform_.Get());
	}
}

void ADXTextRenderer::StaticInitialize()
{
	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//三次元座標
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//法線ベクトル
			"NORMAL",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		},
		{//uv座標
			"TEXCOORD",
			0,
			DXGI_FORMAT_R32G32_FLOAT,
			0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
			0
		}
	};

	Microsoft::WRL::ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト

	LoadShader(&vsBlob, L"Resources/shader/OBJVertexShader.hlsl", "vs_5_0");
	LoadShader(&psBlob, L"Resources/shader/OBJPixelShader.hlsl", "ps_5_0");

	//グラフィックスパイプラインの設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc =
		CreateDefaultPipelineDesc(vsBlob.Get(), psBlob.Get(), inputLayout, _countof(inputLayout));

	CreateGraphicsPipelineState(&pipelineDesc, &S_pipelineState);
}

void ADXTextRenderer::AddFont(const std::string letters, const std::string& folderPath, const std::string& largeLetterFolderPath,
	const std::vector<charAndString>& translateFileNameDatas)
{
	font* targetFont = nullptr;

	//既に設定されている場合は上書き、そうでなければ新規作成
	bool hitted = false;
	for (auto& itr : S_fonts)
	{
		if (itr.name == folderPath)
		{
			targetFont = &itr;
			hitted = true;
			break;
		}
	}
	if (!hitted)
	{
		S_fonts.push_back(font());
		targetFont = &S_fonts.back();
	}
	
	//一文字ずつ生成
	for (auto& itr : letters)
	{
		std::string fileName = "";
		fileName += itr;
		//記号など、対応させたい文字とファイル名が違うものは変換
		for (auto& transItr : translateFileNameDatas)
		{
			if (transItr.c == itr)
			{
				fileName = transItr.str;
				break;
			}
		}
		//大文字はフォルダ分けされているのでそのフォルダ名をファイルパスに追加
		if (std::isupper(itr) != 0)
		{
			fileName = largeLetterFolderPath + "/" + fileName;
		}

		fileName = folderPath + "/" + fileName + ".png";
		AddLetter(targetFont, { ADXImage::LoadADXImage(fileName), itr });
	}

	targetFont->name = folderPath;
}

ADXTextRenderer::font* ADXTextRenderer::GetFont(const std::string fontName)
{
	//同じ名前のフォントを検索し、ヒットしたらそれを返す
	for (auto& itr : S_fonts)
	{
		if (itr.name == fontName)
		{
			return &itr;
			break;
		}
	}

	return nullptr;
}

void ADXTextRenderer::AddLetter(font* targetFont, const fontLetter& setLetter)
{
	//既に設定されている文字は上書き、そうでなければ新規作成
	bool hitted = false;
	for (auto& itr : targetFont->fontDataCells_)
	{
		if (setLetter.character == itr.character)
		{
			itr = setLetter;
			hitted = true;
			break;
		}
	}
	if (!hitted)
	{
		targetFont->fontDataCells_.push_back(setLetter);
	}
}
