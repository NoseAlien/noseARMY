#include "ADXSceneManager.h"
#include <typeinfo>

ADXKeyBoardInput* ADXSceneManager::S_keyboard = nullptr;
ADXScene* ADXSceneManager::S_prevScene = nullptr;
ADXScene* ADXSceneManager::S_currentScene = nullptr;

ADXScene ADXSceneManager::S_scene;
GameScene ADXSceneManager::S_gameScene;

int32_t ADXSceneManager::S_prevSceneNum = -1;
int32_t ADXSceneManager::S_sceneNum = 0;
bool ADXSceneManager::S_reload = true;


void ADXSceneManager::StaticInitialize(ADXKeyBoardInput* setKeyboard)
{
	S_keyboard = setKeyboard;
}

void ADXSceneManager::StaticUpdate()
{
	switch (S_sceneNum)
	{
	case 0:
		S_currentScene = &S_scene;
		break;
	case 1:
		S_currentScene = &S_scene;
		break;
	case 2:
		S_currentScene = &S_gameScene;
		break;
	}


	if (S_currentScene != S_prevScene || S_sceneNum != S_prevSceneNum || S_reload)
	{
		*S_currentScene = {};
		S_currentScene->Initialize();

		S_reload = false;
		S_prevSceneNum = S_sceneNum;
		S_prevScene = S_currentScene;
	}
	else
	{
		S_currentScene->Update();
	}
}