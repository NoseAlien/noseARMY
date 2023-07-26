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
#include "Goal.h"
#include "ADXImage.h"
#include "ADXTexAnimation.h"

class GameScene : public ADXScene
{
private:
	//�摜
	uint32_t keyImg{};
	uint32_t MEGNOSE_Img{};
	uint32_t skyDomeImg{};
	uint32_t groundImg{};
	uint32_t battleFieldImg{};

	//3D���f��
	ADXModel cube{};
	ADXModel ground{};
	ADXModel battleBox{};
	ADXModel rect{};
	ADXModel playerModel{};
	ADXModel skyDomeModel{};

	//�}�e���A��
	ADXMaterial material{};
	ADXMaterial unlitMat{};

	//�J����
	ADXCamera camera_{};

	//�I�u�W�F�N�g
	ADXObject key{};
	Player player_{};
	std::list<ADXObject> floors_{};
	std::list<TutorialArea> tutorialAreas_{};
	std::list<FieldBox> fields_{};
	std::list<Enemy> enemies_{};
	Goal goal_{};

	ADXObject backGround_{};

	//�S�ẴI�u�W�F�N�g
	std::list<ADXObject*> objs{};

public:
	GameScene();
	void Initialize();
	void Update();
};