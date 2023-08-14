#pragma once

#include "ADXScene.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "FieldBox.h"
#include "ADXTexAnimation.h"

class StageSelectScene : public ADXScene
{
private:
	//�摜
	uint32_t keyImg{};
	uint32_t backGroundTex{};
	uint32_t skyDomeImg{};
	uint32_t groundImg{};
	uint32_t battleFieldImg{};
	uint32_t goalImg{};

	//3D���f��
	ADXModel cube{};
	ADXModel ground{};
	ADXModel battleBox{};
	ADXModel rect{};
	ADXModel playerModel{};
	ADXModel skyDomeModel{};

	//�}�e���A��
	ADXMaterial unlitMat{};

	//�J����
	ADXCamera camera_{};

	//�I�u�W�F�N�g
	ADXObject key{};
	Player player_{};
	std::list<ADXObject> floors_{};
	std::list<TutorialArea> tutorialAreas_{};
	std::list<FieldBox> fields_{};

	ADXObject backGround_{};

	//�S�ẴI�u�W�F�N�g
	std::list<ADXObject*> objs{};

public:
	StageSelectScene();
	void Initialize();
	void Update();
};