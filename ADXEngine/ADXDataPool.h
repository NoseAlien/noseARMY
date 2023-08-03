#pragma once
#include <string>
#include <vector>
#include "ADXImage.h"
#include "ADXModel.h"

class ADXDataPool
{
private:
	static std::vector<ADXImage> S_imgDataPool;
	static std::vector<ADXModel> S_modelDataPool;

public:
	static std::vector<ADXImage> GetImgDataPool() { return S_imgDataPool; };
	static ADXImage* GetImgData(const uint32_t& GHandle);
	static void SetImgDataPool(const ADXImage& cell);

	static std::vector<ADXModel> GetModelDataPool() { return S_modelDataPool; };
	static void SetImgDataPool(const ADXModel& cell);

};
