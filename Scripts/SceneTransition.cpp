﻿#include "SceneTransition.h"

bool SceneTransition::S_sceneChanging = false;
int32_t SceneTransition::S_sceneChangeFrame = 0;
int32_t SceneTransition::S_nextSceneNum = 0;
ADXModel SceneTransition::S_rect{};

void SceneTransition::UniqueInitialize()
{
	GetGameObject()->transform.rectTransform = true;
	GetGameObject()->model = &S_rect;
	GetGameObject()->texture = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	GetGameObject()->renderLayer = 10;
}

void SceneTransition::UniqueUpdate()
{
	float shutterScale = sinf(((float)S_sceneChangeFrame / S_MaxSceneChangeFrame) * ADXUtility::Pi) * 2;
	GetGameObject()->transform.localScale_ = { shutterScale ,shutterScale * ADXWindow::GetAspect() ,shutterScale };
}

void SceneTransition::StaticInitialize()
{
	S_rect = ADXModel::CreateRect();
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