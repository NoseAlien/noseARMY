#include "ADXDataPool.h"

std::vector<ADXImage> ADXDataPool::S_imgDataPool{};

ADXImage* ADXDataPool::GetImgData(const uint32_t& GHandle)
{
	for (uint32_t i = 0; i < S_imgDataPool.size(); i++)
	{
		if (S_imgDataPool[i].GetGHandle() == GHandle)
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
