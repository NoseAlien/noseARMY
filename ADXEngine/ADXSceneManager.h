#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXScene.h"

class ADXSceneManager
{
private:
	ADXScene* prevScene = nullptr;
	ADXScene* currentScene = nullptr;

	std::vector<ADXScene*> scenes{};

	int32_t prevSceneIndex = -1;
	int32_t sceneIndex = 0;
	bool reload = true;
	bool initializable = true;

	static ADXSceneManager* S_current;

public:
	void Initialize();
	void Update();

	int32_t GetSceneNum() { return sceneIndex; };
	void SetSceneNum(int32_t setSceneNum) { sceneIndex = setSceneNum; };

	static ADXSceneManager* GetCurrentInstance() { return S_current; };

protected:
	virtual void UniqueInitialize() {};

	void SetScenes(const std::vector<ADXScene*>& setScenes);
};