#include "ADXSceneManager.h"

ADXSceneManager* ADXSceneManager::S_current = nullptr;


void ADXSceneManager::Initialize()
{
	if (!initializable)
	{
		return;
	}
	sceneIndex = 0;
	S_current = this;

	UniqueInitialize();
	initializable = false;
}

void ADXSceneManager::Update()
{
	initializable = false;
	currentScene = scenes[sceneIndex];

	if (currentScene != prevScene || sceneIndex != prevSceneIndex || reload)
	{
		*currentScene = {};
		currentScene->Initialize();

		reload = false;
		prevSceneIndex = sceneIndex;
		prevScene = currentScene;
	}
	else
	{
		currentScene->Update();
	}

	S_current = this;
}

void ADXSceneManager::SetScenes(std::vector<ADXScene*> setScenes)
{
	if (!initializable)
	{
		return;
	}
	scenes = setScenes;
}
