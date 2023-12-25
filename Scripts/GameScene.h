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
	//画像
	uint32_t keyImg_ = 0;
	uint32_t backGroundTex_ = 0;
	uint32_t groundImg_ = 0;

	//3Dモデル
	ADXModel ground_{};
	ADXModel rect_{};

	//カメラ
	ADXCamera* camera_{};

	//オブジェクト
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