#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXScene.h"
#include "GameScene.h"

class ADXSceneManager
{
private:
	static ADXKeyBoardInput* S_keyboard;
	static ADXScene* S_prevScene;
	static ADXScene* S_currentScene;

	static ADXScene S_scene;
	static GameScene S_gameScene;

	static int32_t S_prevSceneNum;
	static int32_t S_sceneNum;
	static bool S_reload;

public:
	static void StaticInitialize(ADXKeyBoardInput* setKeyboard);
	static void StaticUpdate();
	static ADXKeyBoardInput* GetKeyboardInput() { return S_keyboard; };

	static int32_t GetSceneNum() { return S_sceneNum; };
	static void SetSceneNum(int32_t setSceneNum) { S_sceneNum = setSceneNum; };
};
