#include "SceneTransition.h"
#include "ADXModelRenderer.h"

bool SceneTransition::S_sceneChanging = false;
int32_t SceneTransition::S_sceneChangeFrame = 0;
int32_t SceneTransition::S_nextSceneNum = 0;
ADXModel SceneTransition::S_rect{};

void SceneTransition::UniqueInitialize()
{
	GetGameObject()->transform_.rectTransform_ = true;
	ADXModelRenderer* tempRenderer = GetGameObject()->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	tempRenderer->model_ = &S_rect;
	GetGameObject()->renderLayer_ = 10;
}

void SceneTransition::UniqueUpdate()
{
	float shutterScale = sinf(((float)S_sceneChangeFrame / S_MaxSceneChangeFrame) * ADXUtility::Pi) * 2;
	GetGameObject()->transform_.localScale_ = { shutterScale ,shutterScale * ADXWindow::GetInstance()->GetAspect() ,shutterScale };
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
			ADXSceneManager::GetCurrentInstance()->SetSceneIndex(S_nextSceneNum);
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