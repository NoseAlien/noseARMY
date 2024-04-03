#pragma once

#include "ADXObject.h"
#include "ADXSceneManager.h"

//シーン遷移演出
class SceneTransition : public ADXComponent
{
private:
	static const int32_t S_MaxSceneChangeFrame = 30;

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void UniqueInitialize();

	//更新処理
	void UniqueUpdate();

	//---以上の関数は必要な時に自動で呼び出される---
private:
	static bool S_sceneChanging;
	static int32_t S_sceneChangeFrame;
	static int32_t S_nextSceneNum;

public:
	//このクラスの更新処理
	static void StaticUpdate();

	//シーン遷移
	static void ChangeScene(int32_t sceneNum);
};