#pragma once
#include "ADXSceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"

class SceneManager : public ADXSceneManager
{
private:
	ADXScene logoScene{};
	TitleScene titleScene{};
	GameScene gameScene{};

private:
	void UniqueInitialize();
};