#pragma once

#include <vector>
#include "ADXImage.h"

class ADXTexAnimation
{
public:
	std::vector<uint32_t> textures{};
	bool loop = true;
	uint32_t delayFrame = 0;

private:
	uint32_t nowDelayFrame = 0;
	uint32_t nowTexIndex = 0;

public:
	ADXTexAnimation Initialize(const std::vector<uint32_t>& setTextures, const uint32_t& setDelayFrame, bool setLoop);
	uint32_t GetNowTex();
	void SetIndex(const uint32_t& index);
	uint32_t GetIndex();
	uint32_t GetLength();
	void Update();
	bool AnimEnd();
};
