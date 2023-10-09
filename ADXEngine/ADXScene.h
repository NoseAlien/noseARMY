#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "ADXImage.h"
#include "ADXAudio.h"

class ADXScene
{
private:
	//画像
	uint32_t engineLogoImg = 0;
	uint32_t logoImg_A = 0;
	uint32_t logoImg_D = 0;
	uint32_t logoImg_X = 0;
	uint32_t logoImg_Engine = 0;
	uint32_t titleImg = 0;

	//音声データ
	ADXAudio logoJingle{};

	//3Dモデル
	ADXModel rect{};

	//マテリアル
	ADXMaterial unlitMat{};

	//カメラ
	ADXCamera* camera_ = nullptr;

	//オブジェクト
	ADXObject* engineLogo = nullptr;
	ADXObject* logo_A = nullptr;
	ADXObject* logo_D = nullptr;
	ADXObject* logo_X = nullptr;
	ADXObject* logo_Engine = nullptr;
	ADXObject* title = nullptr;

	int32_t startTime = 0;

public:
	ADXScene();
	virtual void Initialize();
	virtual void Update();
};