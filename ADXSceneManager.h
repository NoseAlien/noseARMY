#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXScene.h"

#include "GameScene.h"

class ADXSceneManager
{
private:
	static ADXKeyBoardInput* keyboard;
	static GameScene gameScene_;

	static std::list<ADXScene*> scenes;
	static ADXScene* prevScene;
	static ADXScene* nowScene;

public:
	static void Update();
	static void SetKeyboardInput(ADXKeyBoardInput* setKeyboard) { keyboard = setKeyboard; };
	static ADXKeyBoardInput* GetKeyboardInput() { return keyboard; };
};
