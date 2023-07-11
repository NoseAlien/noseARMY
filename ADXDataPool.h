#pragma once
#include <string>
#include <vector>
#include "ADXImage.h"

class ADXDataPool
{
private:
	static std::vector<ADXImage> S_imgDataPool;

public:
	static std::vector<ADXImage> GetImgDataPool() { return S_imgDataPool; };
	static ADXImage* GetImgData(const uint32_t& GHandle);
	static void SetImgDataPool(const ADXImage& cell);

};
