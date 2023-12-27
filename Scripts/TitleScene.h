#pragma once
#include "ADXScene.h"
#include "SceneTransition.h"

//タイトルシーン
class TitleScene : public ADXScene
{
private:
	uint32_t titleImg_ = 0;
	uint32_t backGroundTex_ = 0;
	uint32_t keyImg_ = 0;

	ADXModel rect_{};

	ADXMaterial unlitMat_{};

	SceneTransition* shutter_ = nullptr;
	ADXObject* title_ = nullptr;
	ADXObject* key_ = nullptr;
	ADXObject* backGround_ = nullptr;
	ADXCamera* camera_ = nullptr;

public:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void Initialize();

	//更新処理
	void Update();

	//---以上の関数は必要な時に自動で呼び出される---
};