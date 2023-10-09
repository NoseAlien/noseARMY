#include "SceneManager.h"

void SceneManager::UniqueInitialize()
{
	SetScenes({ &logoScene,&titleScene,&stageSelectScene,&gameScene });
}