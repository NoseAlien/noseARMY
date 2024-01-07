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

//プレイ画面のシーン
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
	uint32_t keyImg = 0;
	uint32_t backGroundTex = 0;
	uint32_t groundImg = 0;
	//uint32_t constructionSignImg = 0;
	uint32_t constructionInfoImg = 0;

	//3Dモデル
	ADXModel ground_{};
	ADXModel rect_{};

	//オブジェクト
	SceneTransition* shutter_ = nullptr;
	ADXObject* key_ = nullptr;

public:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//---以上の関数は必要な時に自動で呼び出される---
};