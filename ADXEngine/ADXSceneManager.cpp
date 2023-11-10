#include "ADXSceneManager.h"

ADXSceneManager* ADXSceneManager::S_current = nullptr;


void ADXSceneManager::Initialize()
{
	if (!initializable_)
	{
		return;
	}
	sceneIndex_ = 0;
	S_current = this;

	UniqueInitialize();
	initializable_ = false;
}

void ADXSceneManager::Update()
{
	initializable_ = false;
	currentScene_ = scenes_[sceneIndex_];

	if (currentScene_ != prevScene_ || sceneIndex_ != prevSceneIndex_ || reload_)
	{
		*currentScene_ = {};
		ADXObject::Annihilate();
		currentScene_->Initialize();

		reload_ = false;
		prevSceneIndex_ = sceneIndex_;
		prevScene_ = currentScene_;
	}
	else
	{
		currentScene_->Update();
	}

	S_current = this;
}

void ADXSceneManager::SetScenes(const std::vector<ADXScene*>& setScenes)
{
	if (!initializable_)
	{
		return;
	}
	scenes_ = setScenes;
}
