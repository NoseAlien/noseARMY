#pragma once

#include "ADXScene.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "Gimmicks/FieldBox.h"
#include "ADXTexAnimation.h"
#include "Gimmicks/SceneGate.h"
#include "SceneTransition.h"

//ステージ選択シーン
class StageSelectScene : public ADXScene
{
private:
	//画像
	uint32_t keyImg_{};
	uint32_t backGroundTex_{};
	uint32_t groundImg_{};

	//3Dモデル
	ADXModel ground_{};
	ADXModel rect_{};

	//オブジェクト
	SceneTransition* shutter_ = nullptr;
	ADXObject* key_ = nullptr;
	Player* player_ = nullptr;
	std::list<ADXObject*> floors_{};
	std::list<TutorialArea*> tutorialAreas_{};
	std::list<FieldBox*> fields_{};
	std::list<SceneGate*> gates_{};

	ADXObject* backGround_{};

public:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//---以上の関数は必要な時に自動で呼び出される---
};