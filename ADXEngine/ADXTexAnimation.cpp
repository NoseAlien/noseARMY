#include "ADXTexAnimation.h"

ADXTexAnimation ADXTexAnimation::Initialize(const std::vector<uint32_t>& setTextures, uint32_t setDelayFrame, bool setLoop)
{
	textures_ = setTextures;
	loop_ = setLoop;
	delayFrame_ = setDelayFrame;

	return *this;
}

void ADXTexAnimation::Update()
{
	if (nowDelayFrame_ > 0)
	{
		nowDelayFrame_--;
	}
	else
	{
		nowTexIndex_++;
		nowDelayFrame_ = delayFrame_;
	}
	if (nowTexIndex_ >= textures_.size())
	{
		if (loop_)
		{
			nowTexIndex_ = 0;
		}
		else
		{
			nowTexIndex_ = (uint32_t)textures_.size() - 1;
		}
	}
}

uint32_t ADXTexAnimation::GetNowTex()
{
	return textures_[nowTexIndex_];
}

void ADXTexAnimation::SetIndex(uint32_t index)
{
	nowTexIndex_ = index;
}

uint32_t ADXTexAnimation::GetIndex()
{
	return nowTexIndex_;
}

uint32_t ADXTexAnimation::GetLength()
{
	return (uint32_t)textures_.size();
}

bool ADXTexAnimation::AnimEnd()
{
	return nowTexIndex_ >= textures_.size() - 1 && nowDelayFrame_ <= 0 && !loop_;
}