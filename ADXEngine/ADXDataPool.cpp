#include "ADXDataPool.h"

std::vector<ADXImage> ADXDataPool::S_imgDataPool{};
//std::vector<ADXAudio> ADXDataPool::S_audioDataPool{};

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

//ADXAudio* ADXDataPool::GetAudioData(const uint32_t& index)
//{
//	return &S_audioDataPool[index];
//}
//
//void ADXDataPool::SetAudioDataPool(const ADXAudio& cell)
//{
//	S_audioDataPool.push_back(cell);
//}
