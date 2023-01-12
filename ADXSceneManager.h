#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXScene.h"

#include "TitleScene.h"
#include "GameScene.h"

class ADXSceneManager
{
private:
	static ADXKeyBoardInput* keyboard;
	static std::unique_ptr<TitleScene> titleScene_;
	static std::unique_ptr<GameScene> gameScene_;

	static int sceneNum;
	static int prevSceneNum;
	static ADXScene* nowScene;
	static bool reload;

public:
	static void Update();
	static void SetKeyboardInput(ADXKeyBoardInput* setKeyboard) { keyboard = setKeyboard; };
	static ADXKeyBoardInput* GetKeyboardInput() { return keyboard; };
};
