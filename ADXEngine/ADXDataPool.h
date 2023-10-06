#pragma once
#include <string>
#include <vector>
#include "ADXImage.h"
#include "ADXAudio.h"
#include "ADXModel.h"

class ADXDataPool
{
private:
	static std::vector<ADXImage> S_imgDataPool;
	static std::vector<ADXAudio> S_audioDataPool;
	static std::vector<ADXModel> S_modelDataPool;

public:
	static std::vector<ADXImage> GetImgDataPool() { return S_imgDataPool; };
	static ADXImage* GetImgData(const uint32_t& gHandle);
	static void SetImgDataPool(const ADXImage& cell);
};
