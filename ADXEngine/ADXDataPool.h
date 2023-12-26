#pragma once
#include <string>
#include <vector>
#include "ADXImage.h"
#include "ADXAudio.h"
#include "ADXModel.h"

//読み込んだリソースを保管するクラス
class ADXDataPool
{
private:
	static std::vector<ADXImage> S_imgDataPool;
	static std::vector<ADXAudio> S_audioDataPool;
	static std::vector<ADXModel> S_modelDataPool;

public:
	//読み込んだ画像データを全て取得
	static std::vector<ADXImage> GetImgDataPool() { return S_imgDataPool; };

	//読み込んだ画像データを名前から検索して取得
	static ADXImage* GetImgData(const uint32_t& gHandle);
	
	//画像をS_imgDataPoolに追加
	static void SetImgDataPool(const ADXImage& cell);
};
