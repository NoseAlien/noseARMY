#pragma once
#include <string>
#include <vector>
#include "ADXImage.h"
#include "ADXModel.h"

namespace ADXEngine
{
	//読み込んだリソースを保管するクラス
	class ADXDataPool
	{
	private:
		static std::vector<ADXImage> S_imgDataPool;
		static std::vector<ADXModel> S_modelDataPool;

	public:
		//読み込んだ画像データを全て取得
		static std::vector<ADXImage> GetImgDataPool() { return S_imgDataPool; };

		//読み込んだ画像データをハンドルから検索して取得
		static ADXImage* GetImgData(const uint32_t& gHandle);

		//画像をS_imgDataPoolに追加
		static void SetImgDataPool(const ADXImage& cell);

		//読み込んだ画像データを全て取得
		static std::vector<ADXModel> GetModelDataPool() { return S_modelDataPool; };

		//読み込んだ画像データを名前から検索して取得
		static ADXModel* GetModelData(const std::string& gHandle);

		//画像をS_imgDataPoolに追加
		static void SetModelDataPool(const ADXModel& cell);
	};
}