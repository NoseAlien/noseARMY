#include "ADXTexAnimation.h"

ADXTexAnimation ADXTexAnimation::Initialize(std::vector<ADXImage> setTextures, int setDelayFrame, bool setLoop)
{
	textures = setTextures;
	loop = setLoop;
	delayFrame = setDelayFrame;

	return *this;
}

void ADXTexAnimation::Update()
{
	if (nowDelayFrame > 0)
	{
		nowDelayFrame--;
	}
	else
	{
		nowTexIndex++;
		nowDelayFrame = delayFrame;
	}
	if (nowTexIndex >= textures.size())
	{
		if (loop)
		{
			nowTexIndex = 0;
		}
		else
		{
			nowTexIndex = textures.size() - 1;
		}
	}
}

ADXImage ADXTexAnimation::GetNowTex()
{
	return textures[nowTexIndex];
}

void ADXTexAnimation::SetIndex(int index)
{
	nowTexIndex = index;
}

int ADXTexAnimation::GetIndex()
{
	return nowTexIndex;
}

int ADXTexAnimation::GetLength()
{
	return textures.size();
}

bool ADXTexAnimation::AnimEnd()
{
	return nowTexIndex >= textures.size() - 1 && nowDelayFrame <= 0 && !loop;
}