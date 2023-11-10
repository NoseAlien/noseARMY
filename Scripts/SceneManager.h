#pragma once
#include "ADXSceneManager.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "GameScene.h"

class SceneManager : public ADXSceneManager
{
private:
	ADXScene logoScene_{};
	TitleScene titleScene_{};
	StageSelectScene stageSelectScene_{};
	GameScene gameScene_{};

private:
	void UniqueInitialize();
};