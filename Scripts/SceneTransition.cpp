#include "SceneTransition.h"

bool SceneTransition::S_sceneChanging = false;
int32_t SceneTransition::S_sceneChangeFrame = 0;
int32_t SceneTransition::S_nextSceneNum = 0;
ADXObject SceneTransition::S_shutter{};
ADXModel SceneTransition::S_rect{};

void SceneTransition::StaticInitialize()
{
	S_rect = ADXModel::CreateRect();

	S_shutter.Initialize();
	S_shutter.transform.rectTransform = true;
	S_shutter.transform.UpdateMatrix();
	S_shutter.model = &S_rect;
	S_shutter.texture = ADXImage::LoadADXImage("apEGnose.png");
	S_shutter.renderLayer = 10;
}

void SceneTransition::StaticUpdate()
{
	if (S_sceneChanging)
	{
		S_sceneChangeFrame++;
		if (S_sceneChangeFrame == S_MaxSceneChangeFrame / 2)
		{
			ADXSceneManager::GetCurrentInstance()->SetSceneNum(S_nextSceneNum);
		}
		if (S_sceneChangeFrame == S_MaxSceneChangeFrame)
		{
			S_sceneChanging = false;
		}

		float shutterScale = sinf(((float)S_sceneChangeFrame / S_MaxSceneChangeFrame) * 3.1415f) * 2;

		S_shutter.transform.localScale_ = { shutterScale ,shutterScale * ADXWindow::GetAspect() ,shutterScale };

		S_shutter.transform.UpdateMatrix();
		S_shutter.Update();
	}
	else
	{
		S_sceneChangeFrame = 0;
	}
}

void SceneTransition::ChangeScene(int32_t sceneNum)
{
	if (!S_sceneChanging)
	{
		S_nextSceneNum = sceneNum;
		S_sceneChanging = true;
		S_sceneChangeFrame = 0;
	}
}