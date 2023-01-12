#include "ADXSceneManager.h"
#include <typeinfo>

ADXKeyBoardInput* ADXSceneManager::keyboard = nullptr;
std::unique_ptr<GameScene> ADXSceneManager::gameScene_{};
int ADXSceneManager::prevSceneNum = 0;
int ADXSceneManager::sceneNum = 0;
ADXScene* ADXSceneManager::nowScene = nullptr;
bool ADXSceneManager::reload = true;

void ADXSceneManager::Update()
{
	switch (sceneNum)
	{
	case 0:
		if (gameScene_ == nullptr)
		{
			gameScene_ = std::make_unique<GameScene>();
			gameScene_->Initialize();
		}
		nowScene = gameScene_.get();
		break;
	}

	if (nowScene != nullptr)
	{
		if (sceneNum == prevSceneNum && !reload)
		{
			nowScene->Update();
		}
		else
		{
			gameScene_.release();
			reload = false;
		}
	}
	prevSceneNum = sceneNum;

	if (ADXSceneManager::GetKeyboardInput()->KeyTrigger(DIK_Q))
	{
		reload = true;
	}
}