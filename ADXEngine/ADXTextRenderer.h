#pragma once

#include <string>
#include <vector>
#include <list>
#include "ADXComponent.h"

class ADXTextRenderer : public ADXComponent
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

private:
	std::vector<fontAndChar> fonts_{};
	std::list<ADXObject*> letters_{};

public:
	void AddFonts(const std::vector<fontAndChar>& fontSet);

private:
	void UniqueUpdate();
	void SafetyPhase();
};