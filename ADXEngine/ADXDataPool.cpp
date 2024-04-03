#include "ADXDataPool.h"

using namespace ADXEngine;

std::vector<ADXImage> ADXDataPool::S_imgDataPool{};
std::vector<ADXModel> ADXDataPool::S_modelDataPool{};

ADXImage* ADXDataPool::GetImgData(const uint32_t& gHandle)
{
	//既に登録したデータを一つずつ検索、ヒットしたら即座にそれを返す
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

ADXModel* ADXEngine::ADXDataPool::GetModelData(const std::string& name)
{
	//既に登録したデータを一つずつ検索、ヒットしたら即座にそれを返す
	for (uint32_t i = 0; i < S_modelDataPool.size(); i++)
	{
		if (S_modelDataPool[i].GetName() == name)
		{
			return &S_modelDataPool[i];
		}
	}
	return nullptr;
}

void ADXEngine::ADXDataPool::SetModelDataPool(const ADXModel& cell)
{
	S_modelDataPool.push_back(cell);
}
