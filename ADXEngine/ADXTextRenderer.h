#pragma once

#include <string>
#include <vector>
#include <list>
#include "ADXRenderer.h"
#include "ADXModel.h"

//画像をフォントとして表示するコンポーネント
class ADXTextRenderer : public ADXRenderer
{
public:
	enum anchor
	{
		upperLeft,
		upperCenter,
		upperRight,
		middleLeft,
		middleCenter,
		middleRight,
		lowerLeft,
		lowerCenter,
		lowerRight,
	};

public:
	struct fontAndChar
	{
		uint32_t font = 0;
		char character = ' ';
	};

public:
	std::string text_ = "";
	float fontSize_ = 1;
	float fontExtend_ = 1;
	float fontAspect_ = 1;
	anchor anchor_{};

private:
	std::vector<fontAndChar> fonts_{};
	ADXModel model_ = ADXModel::CreateRect();
	std::vector<ADXWorldTransform>fontWtfs_;

public:
	//フォントを追加
	void AddFonts(const std::vector<fontAndChar>& fontSet);

	//文字からフォント画像を検索して追加
	uint32_t GetFontTex(const char& character);

private:
	//描画処理
	void UniqueRendering(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
};