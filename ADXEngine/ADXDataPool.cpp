#include "ADXDataPool.h"

std::vector<ADXImage> ADXDataPool::S_imgDataPool{};
std::vector<ADXAudio> ADXDataPool::S_audioDataPool{};
std::vector<ADXModel> ADXDataPool::S_modelDataPool{};

ADXImage* ADXDataPool::GetImgData(const uint32_t& gHandle)
{
	for (uint32_t i = 0; i < S_imgDataPool.size(); i++)
	{
		if (S_imgDataPool[i].GetGHandle() == gHandle)
		{
			return &S_imgDataPool[i];
		}
	}
	return nullptr;
}

void ADXDataPool::SetImgDataPool(const ADXImage& cell)
{
	S_imgDataPool.push_back(cell);
}