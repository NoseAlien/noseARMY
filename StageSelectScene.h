#pragma once

#include "ADXScene.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "FieldBox.h"
#include "ADXTexAnimation.h"
#include "SceneGate.h"

class StageSelectScene : public ADXScene
{
private:
	//画像
	uint32_t keyImg{};
	uint32_t backGroundTex{};
	uint32_t skyDomeImg{};
	uint32_t groundImg{};
	uint32_t battleFieldImg{};
	uint32_t goalImg{};

	//3Dモデル
	ADXModel cube{};
	ADXModel ground{};
	ADXModel battleBox{};
	ADXModel rect{};
	ADXModel playerModel{};
	ADXModel skyDomeModel{};

	//カメラ
	ADXCamera camera_{};

	//オブジェクト
	ADXObject key{};
	Player player_{};
	std::list<ADXObject> floors_{};
	std::list<TutorialArea> tutorialAreas_{};
	std::list<FieldBox> fields_{};
	std::list<SceneGate> gates_{};

	ADXObject backGround_{};

	//全てのオブジェクト
	std::list<ADXObject*> objs{};

public:
	StageSelectScene();
	void Initialize();
	void Update();
};