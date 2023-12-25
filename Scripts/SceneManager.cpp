#include "SceneManager.h"

void SceneManager::UniqueInitialize()
{
	SetScenes({ &logoScene_,&titleScene_,&stageSelectScene_,&gameScene_ });
}