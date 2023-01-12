#include "ADXSceneManager.h"

ADXKeyBoardInput* ADXSceneManager::keyboard = nullptr;
GameScene ADXSceneManager::gameScene_{};
std::list<ADXScene*> ADXSceneManager::scenes{&gameScene_};
ADXScene* ADXSceneManager::prevScene = nullptr;
ADXScene* ADXSceneManager::nowScene = nullptr;

void ADXSceneManager::Update()
{
	if (nowScene != nullptr)
	{
		if (nowScene == prevScene)
		{
			nowScene->Update();
		}
		else
		{
			nowScene->Initialize();
		}
	}
	prevScene = nowScene;
	if (nowScene == nullptr)
	{
		nowScene = *scenes.begin();
	}
}