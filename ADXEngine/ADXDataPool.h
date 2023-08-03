#pragma once
#include <string>
#include <vector>
#include "ADXImage.h"
#include "ADXAudio.h"

class ADXDataPool
{
private:
	static std::vector<ADXImage> S_imgDataPool;
	//static std::vector<ADXAudio> S_audioDataPool;

public:
	static std::vector<ADXImage> GetImgDataPool() { return S_imgDataPool; };
	static ADXImage* GetImgData(const uint32_t& gHandle);
	static void SetImgDataPool(const ADXImage& cell);

	//static std::vector<ADXAudio> GetAudioDataPool() { return S_audioDataPool; };
	//static ADXAudio* GetAudioData(const uint32_t& index);
	//static void SetAudioDataPool(const ADXAudio& cell);
};
