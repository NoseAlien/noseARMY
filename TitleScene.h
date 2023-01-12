#pragma once

#include "ADXScene.h"
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXWorldTransform.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "FieldBox.h"
#include "ADXImage.h"
#include "ADXTexAnimation.h"

class TitleScene : public ADXScene
{
private:
	//画像
	ADXImage titleImg{};

	//3Dモデル
	ADXModel rect{};

	//マテリアル
	ADXMaterial unlitMat{};

	//カメラ
	ADXCamera camera_{};

	//オブジェクト
	ADXObject title{};

	//全てのオブジェクト
	std::list<ADXObject*> objs{};

public:
	TitleScene();
	void Initialize();
	void Update();
};