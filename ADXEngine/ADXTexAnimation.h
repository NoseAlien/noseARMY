#pragma once

#include <vector>
#include "ADXImage.h"

class ADXTexAnimation
{
public:
	std::vector<uint32_t> textures_{};
	bool loop_ = true;
	uint32_t delayFrame_ = 0;

private:
	uint32_t nowDelayFrame_ = 0;
	uint32_t nowTexIndex_ = 0;

public:
	ADXTexAnimation Initialize(const std::vector<uint32_t>& setTextures, uint32_t setDelayFrame, bool setLoop);
	uint32_t GetNowTex();
	void SetIndex(uint32_t index);
	uint32_t GetIndex();
	uint32_t GetLength();
	void Update();
	bool AnimEnd();
};
