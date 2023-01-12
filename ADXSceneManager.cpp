#include "ADXSceneManager.h"
#include <typeinfo>

ADXKeyBoardInput* ADXSceneManager::keyboard = nullptr;
std::unique_ptr<TitleScene> ADXSceneManager::titleScene_{};
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
		if (titleScene_ == nullptr)
		{
			titleScene_ = std::make_unique<TitleScene>();
			titleScene_->Initialize();
		}
		nowScene = titleScene_.get();

		if (ADXSceneManager::GetKeyboardInput()->KeyTrigger(DIK_SPACE))
		{
			sceneNum = 1;
		}
		break;
	case 1:
		if (gameScene_ == nullptr)
		{
			gameScene_ = std::make_unique<GameScene>();
			gameScene_->Initialize();
		}
		nowScene = gameScene_.get();

		if (ADXSceneManager::GetKeyboardInput()->KeyTrigger(DIK_Q))
		{
			sceneNum = 0;
		}
		break;
	default:
		sceneNum = 0;
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
}