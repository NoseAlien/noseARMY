#pragma once

#include "ADXObject.h"
#include "ADXSceneManager.h"

class SceneTransition
{
private:
	static const int32_t S_MaxSceneChangeFrame = 30;

private:
	static bool S_sceneChanging;
	static int32_t S_sceneChangeFrame;
	static int32_t S_nextSceneNum;
	static ADXObject* S_shutter;
	static ADXModel S_rect;

public:
	static void StaticInitialize();
	static void StaticUpdate();
	static void ChangeScene(int32_t sceneNum);
};