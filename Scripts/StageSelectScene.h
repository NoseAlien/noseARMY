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
	//�摜
	uint32_t keyImg{};
	uint32_t backGroundTex{};
	uint32_t groundImg{};

	//3D���f��
	ADXModel ground{};
	ADXModel rect{};

	//�J����
	ADXCamera camera_{};

	//�I�u�W�F�N�g
	ADXObject key{};
	Player player_{};
	std::list<ADXObject> floors_{};
	std::list<TutorialArea> tutorialAreas_{};
	std::list<FieldBox> fields_{};
	std::list<SceneGate> gates_{};

	ADXObject backGround_{};

	//�S�ẴI�u�W�F�N�g
	std::list<ADXObject*> objs{};

public:
	StageSelectScene();
	void Initialize();
	void Update();
};