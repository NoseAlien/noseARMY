#pragma once

#include "ADXVector3.h"

class ADXUtility
{
public:
	static ADXVector3 Lerp(ADXVector3 nowPos, ADXVector3 newPos, float progress);
	static float AngleDiff(float angle1, float angle2);
	static float ValueMapping(float nowValue, float inputStart, float inputEnd,
		float outputStart, float outputEnd);
};
