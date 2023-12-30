#include "ADXTexAnimation.h"

ADXTexAnimation ADXTexAnimation::Initialize(const std::vector<uint32_t>& setTextures, uint32_t setDelayFrame, bool setLoop)
{
	//引数から変数の値を設定
	textures_ = setTextures;
	loop_ = setLoop;
	delayFrame_ = setDelayFrame;

	return *this;
}

void ADXTexAnimation::Update()
{
	//時間が経ったら次の画像に切り替える
	if (nowDelayFrame_ > 0)
	{
		nowDelayFrame_--;
	}
	else
	{
		nowTexIndex_++;
		nowDelayFrame_ = delayFrame_;
	}

	//必要に応じてループ
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

bool ADXTexAnimation::IsAnimEnd()
{
	return nowTexIndex_ >= textures_.size() - 1 && nowDelayFrame_ <= 0 && !loop_;
}