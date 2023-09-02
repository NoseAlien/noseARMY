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

ADXAudio* ADXDataPool::GetAudioData(const uint32_t& sHandle)
{
	for (uint32_t i = 0; i < S_audioDataPool.size(); i++)
	{
		if (S_audioDataPool[i].GetSHandle() == sHandle)
		{
			return &S_audioDataPool[i];
		}
	}
	return nullptr;
}

//void ADXDataPool::SetAudioDataPool(const ADXAudio& cell)
//{
//	S_audioDataPool.push_back(cell);
//}

ADXModel* ADXDataPool::GetModelData(const uint32_t& mHandle)
{
	return nullptr;
}

void ADXDataPool::SetModelDataPool(const ADXModel& cell)
{
	S_modelDataPool.push_back(cell);
}