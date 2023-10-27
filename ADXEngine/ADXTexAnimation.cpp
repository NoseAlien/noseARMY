#include "ADXTexAnimation.h"

ADXTexAnimation ADXTexAnimation::Initialize(const std::vector<uint32_t>& setTextures, uint32_t setDelayFrame, bool setLoop)
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
			nowTexIndex = (uint32_t)textures.size() - 1;
		}
	}
}

uint32_t ADXTexAnimation::GetNowTex()
{
	return textures[nowTexIndex];
}

void ADXTexAnimation::SetIndex(uint32_t index)
{
	nowTexIndex = index;
}

uint32_t ADXTexAnimation::GetIndex()
{
	return nowTexIndex;
}

uint32_t ADXTexAnimation::GetLength()
{
	return (uint32_t)textures.size();
}

bool ADXTexAnimation::AnimEnd()
{
	return nowTexIndex >= textures.size() - 1 && nowDelayFrame <= 0 && !loop;
}