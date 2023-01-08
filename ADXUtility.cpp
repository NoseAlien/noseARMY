#include "ADXUtility.h"
#include <math.h>



ADXVector3 ADXUtility::Lerp(ADXVector3 nowPos, ADXVector3 newPos, float progress)
{
	ADXVector3 ret;
	progress = fmax(0, fmin(progress, 1));

	ret = nowPos + ((newPos - nowPos) * progress);
	return ret;
}

float ADXUtility::Lerp(float nowPos, float newPos, float progress)
{
	float ret;
	progress = fmax(0, fmin(progress, 1));

	ret = nowPos + ((newPos - nowPos) * progress);
	return ret;
}

float ADXUtility::EaseIn(float progress, float powNum)
{
	progress = fmax(0, fmin(progress, 1));

	progress = pow(progress, powNum);
	return progress;
}

float ADXUtility::EaseOut(float progress, float powNum)
{
	progress = fmax(0, fmin(progress, 1));

	progress = 1 - pow(1 - progress, powNum);
	return progress;
}

//角度の差を求める　目的の角度へゆっくり向ける時に使ってみよう
float ADXUtility::AngleDiff(float angle1, float angle2)
{
	angle1 = fmod(angle1, 2 * 3.1415);
	angle2 = fmod(angle2, 2 * 3.1415);
	float angle3 = angle2 - 2 * 3.1415;
	float angle4 = angle2 + 2 * 3.1415;

	float nearestAngleDiff = angle2 - angle1;
	if (fabsf(nearestAngleDiff) > fabsf(angle3 - angle1))
	{
		nearestAngleDiff = angle3 - angle1;
	}
	if (fabsf(nearestAngleDiff) > fabsf(angle4 - angle1))
	{
		nearestAngleDiff = angle4 - angle1;
	}
	return nearestAngleDiff;
}

//inputStartからinputEndまでの値をoutputStartからoutputEndまでの値に変換する
float ADXUtility::ValueMapping(float nowValue, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
	return outputStart + (outputEnd - outputStart) * ((nowValue - inputStart) / (inputEnd - inputStart));
}