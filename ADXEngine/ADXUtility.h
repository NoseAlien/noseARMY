#pragma once

#include "ADXVector3.h"
#include <string>

class ADXUtility
{
public:
	static ADXVector3 Lerp(const ADXVector3& startValue, const ADXVector3& endValue, float progress);
	static float Lerp(float startValue, float endValue, float progress);
	static float EaseIn(float progress, float pow);
	static float EaseOut(float progress, float pow);
	static float AngleDiff(float angle1, float angle2);
	static float LerpInverse(float nowValue, float startValue, float endValue);
	static float ValueMapping(float nowValue, float inputStart, float inputEnd,
		float outputStart, float outputEnd);
	static wchar_t* StringToWideChar(const std::string& pKey);
};
