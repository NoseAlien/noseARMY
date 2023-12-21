﻿#pragma once

#include <string>
#include <vector>
#include <list>
#include "ADXRenderer.h"

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
	float fontAspect = 1;
	anchor anchor_{};

private:
	std::vector<fontAndChar> fonts_{};

public:
	void AddFonts(const std::vector<fontAndChar>& fontSet);
	void GetFontTex(const char& character);

private:
	void UniqueRendering(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
};