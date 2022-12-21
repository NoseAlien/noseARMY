#pragma once

#include "ADXImage.h"

class ADXTexAnimation
{
public:
	ADXTexAnimation Initialize(std::vector<ADXImage> setTextures, int setDelayFrame, bool setLoop);
	ADXImage GetNowTex();
	void SetIndex(int index);
	int GetIndex();
	int GetLength();
	void Update();
	bool AnimEnd();
private:
	std::vector<ADXImage> textures{};
	bool loop = true;
	int delayFrame = 0;
	int nowDelayFrame = 0;
	int nowTexIndex = 0;
};
