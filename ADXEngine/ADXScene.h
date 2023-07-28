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
	uint32_t keyImg = 0;

	//音声データ
	ADXAudio logoJingle{};

	//3Dモデル
	ADXModel rect{};

	//マテリアル
	ADXMaterial unlitMat{};

	//カメラ
	ADXCamera camera_{};

	//オブジェクト
	ADXObject engineLogo{};
	ADXObject logo_A{};
	ADXObject logo_D{};
	ADXObject logo_X{};
	ADXObject logo_Engine{};
	ADXObject title{};
	ADXObject key{};

	int32_t startTime = 0;

	//全てのオブジェクト
	std::list<ADXObject*> objs{};

public:
	ADXScene();
	virtual void Initialize();
	virtual void Update();
};