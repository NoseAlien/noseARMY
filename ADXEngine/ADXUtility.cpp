#include "ADXUtility.h"
#include <math.h>
#include <wrl.h>

const float ADXUtility::Pi = 3.1415f;

ADXVector3 ADXUtility::Lerp(const ADXVector3& startValue, const ADXVector3& endValue, float progress)
{
	ADXVector3 ret;
	float currentProgress = fmaxf(0, fminf(progress, 1));

	ret = startValue + ((endValue - startValue) * currentProgress);
	return ret;
}

float ADXUtility::Lerp(float startValue, float endValue, float progress)
{
	float ret;
	float currentProgress = fmaxf(0, fminf(progress, 1));

	ret = startValue + ((endValue - startValue) * currentProgress);
	return ret;
}

float ADXUtility::EaseIn(float progress, float powNum)
{
	float currentProgress = fmaxf(0, fminf(progress, 1));

	currentProgress = (float)pow(currentProgress, powNum);
	return currentProgress;
}

float ADXUtility::EaseOut(float progress, float powNum)
{
	float currentProgress = fmaxf(0, fminf(progress, 1));

	currentProgress = 1 - (float)pow(1 - currentProgress, powNum);
	return currentProgress;
}

float ADXUtility::AngleDiff(float angle1, float angle2)
{
	float angle1Current = fmodf(angle1, 2 * ADXUtility::Pi);
	float angle2Current = fmodf(angle2, 2 * ADXUtility::Pi);
	float angle3 = angle2Current - 2 * ADXUtility::Pi;
	float angle4 = angle2Current + 2 * ADXUtility::Pi;

	float nearestAngleDiff = angle2Current - angle1Current;
	//最小の角度を探す
	if (fabsf(nearestAngleDiff) > fabsf(angle3 - angle1Current))
	{
		nearestAngleDiff = angle3 - angle1Current;
	}
	if (fabsf(nearestAngleDiff) > fabsf(angle4 - angle1Current))
	{
		nearestAngleDiff = angle4 - angle1Current;
	}
	return nearestAngleDiff;
}

float ADXUtility::LerpInverse(float nowValue, float startValue, float endValue)
{
	float ret = nowValue;
	ret -= startValue;
	ret /= endValue - startValue;

	return ret;
}

float ADXUtility::ValueMapping(float nowValue, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	return outputStart + (outputEnd - outputStart) * ((nowValue - inputStart) / (inputEnd - inputStart));
}

wchar_t* ADXUtility::StringToWideChar(const std::string& pKey)
{
	const char* pCStrKey = pKey.c_str();
	//                ，     wchar_t*
	int32_t pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int32_t)strlen(pCStrKey) + 1, NULL, 0);
	wchar_t* pWCStrKey = new wchar_t[pSize];
	//                     
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, (int32_t)strlen(pCStrKey) + 1, pWCStrKey, pSize);
	return pWCStrKey;
}