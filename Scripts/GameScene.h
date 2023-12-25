#pragma once

#include "ADXScene.h"
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXWorldTransform.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "ADXImage.h"
#include "ADXTexAnimation.h"
#include "ADXUtility.h"
#include "SceneTransition.h"
#include "Player.h"
#include "Enemy.h"
#include "FieldBox.h"
#include "BattleFieldBox.h"
#include "Goal.h"

class GameScene : public ADXScene
{
private:
	struct TransformData
	{
		ADXVector3 localPosition{};
		ADXQuaternion localRotation{};
		ADXVector3 localScale{};
	};

private:
	//�摜
	uint32_t keyImg_ = 0;
	uint32_t backGroundTex_ = 0;
	uint32_t groundImg_ = 0;

	//3D���f��
	ADXModel ground_{};
	ADXModel rect_{};

	//�J����
	ADXCamera* camera_{};

	//�I�u�W�F�N�g
	SceneTransition* shutter_ = nullptr;
	Player* player_ = nullptr;
	std::list<ADXObject*> floors_{};
	std::list<FieldBox*> fields_{};
	std::list<BattleFieldBox*> battleFields_{};
	Goal* goal_ = nullptr;
	ADXObject* key_ = nullptr;
	ADXObject* backGround_ = nullptr;

	EnemySpawnData enemySpawnData_{};
	std::list<TransformData> floorGenerateData_{};

public:
	void Initialize();
	void Update();
};