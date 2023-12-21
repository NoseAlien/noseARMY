#pragma once

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
		uint32_t font{};
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

private:
	void UniqueRendering();
};