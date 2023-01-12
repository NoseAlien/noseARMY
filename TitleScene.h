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
	//�摜
	ADXImage titleImg{};

	//3D���f��
	ADXModel rect{};

	//�}�e���A��
	ADXMaterial unlitMat{};

	//�J����
	ADXCamera camera_{};

	//�I�u�W�F�N�g
	ADXObject title{};

	//�S�ẴI�u�W�F�N�g
	std::list<ADXObject*> objs{};

public:
	TitleScene();
	void Initialize();
	void Update();
};