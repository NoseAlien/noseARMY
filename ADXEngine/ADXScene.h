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
	uint32_t engineLogoImg_ = 0;
	uint32_t logoImg_A_ = 0;
	uint32_t logoImg_D_ = 0;
	uint32_t logoImg_X_ = 0;
	uint32_t logoImg_Engine_ = 0;
	uint32_t titleImg_ = 0;

	//音声データ
	ADXAudio logoJingle_{};

	//3Dモデル
	ADXModel rect_{};

	//マテリアル
	ADXMaterial unlitMat_{};

	//カメラ
	ADXCamera* camera_ = nullptr;

	//オブジェクト
	ADXObject* engineLogo_ = nullptr;
	ADXObject* logo_A_ = nullptr;
	ADXObject* logo_D_ = nullptr;
	ADXObject* logo_X_ = nullptr;
	ADXObject* logo_Engine_ = nullptr;
	ADXObject* title_ = nullptr;

	int32_t startTime_ = 0;

public:
	ADXScene();
	virtual void Initialize();
	virtual void Update();
};