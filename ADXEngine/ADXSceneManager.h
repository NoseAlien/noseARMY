#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXScene.h"

class ADXSceneManager
{
private:
	ADXScene* prevScene_ = nullptr;
	ADXScene* currentScene_ = nullptr;

	std::vector<ADXScene*> scenes_{};

	int32_t prevSceneIndex_ = -1;
	int32_t sceneIndex_ = 0;
	bool reload_ = true;
	bool initializable_ = true;

private:
	static ADXSceneManager* S_current;

public:
	void Initialize();
	void Update();

	int32_t GetSceneNum() { return sceneIndex_; };
	void SetSceneNum(int32_t setSceneNum) { sceneIndex_ = setSceneNum; };

protected:
	virtual void UniqueInitialize() {};

	void SetScenes(const std::vector<ADXScene*>& setScenes);

public:
	static ADXSceneManager* GetCurrentInstance() { return S_current; };
};