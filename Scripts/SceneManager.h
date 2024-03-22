#pragma once
#include "ADXSceneManager.h"
#include "Scenes/TitleScene.h"
#include "Scenes/StageSelectScene.h"
#include "Scenes/GameScene.h"

//このゲームのシーンマネージャー
class SceneManager : public ADXSceneManager
{
private:
	ADXScene logoScene_{};
	TitleScene titleScene_{};
	StageSelectScene stageSelectScene_{};
	GameScene gameScene_{};

private:
	//初期化処理
	void UniqueInitialize();
};