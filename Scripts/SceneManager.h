#pragma once
#include "ADXSceneManager.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "GameScene.h"

class SceneManager : public ADXSceneManager
{
private:
	ADXScene logoScene{};
	TitleScene titleScene{};
	StageSelectScene stageSelectScene{};
	GameScene gameScene{};

private:
	void UniqueInitialize();
};