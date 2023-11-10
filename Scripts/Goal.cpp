#include "Goal.h"
#include "LiveEntity.h"
#include "SceneTransition.h"

void Goal::Initialize(const std::string& setTeam)
{
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/goalField.png");
	GetGameObject()->sortingOrder_ = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = box;

	team = setTeam;
	sceneTransitionCount = MaxSceneTransitionCount;

	rectModel = ADXModel::CreateRect();
	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model_ = &boxModel;

	clearUI = ADXObject::Create();
	clearUI->transform_.rectTransform_ = true;
	clearUI->model_ = &rectModel;
	clearUI->texture_ = ADXImage::LoadADXImage("texture/clear.png");
	clearUI->renderLayer_ = 5;

	keyUI = ADXObject::Duplicate(*clearUI);
	keyUI->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
}

void Goal::UniqueUpdate()
{
	clearUI->transform_.localPosition_ = { 0,0.5f + sin(clock() * 0.001f) * 0.02f,0 };
	clearUI->transform_.localScale_ = { clearUI->transform_.localScale_.x_,0.7f,1 };

	keyUI->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI->transform_.localScale_ = { keyUI->transform_.localScale_.x_,0.45f,1 };

	if (sceneTransitionCount != MaxSceneTransitionCount)
	{
		clearUI->transform_.localScale_.x_ += (0.7f / ADXWindow::GetAspect() - clearUI->transform_.localScale_.x_) * 0.3f;
		clearUI->transform_.UpdateMatrix();

		sceneTransitionCount--;
		if (sceneTransitionCount <= 0)
		{
			keyUI->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI->transform_.localScale_.x_) * 0.3f;
			if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
			{
				SceneTransition::ChangeScene(2);
			}
		}
		else
		{
			keyUI->transform_.localScale_.x_ = 0;
		}
	}
	else
	{
		clearUI->transform_.localScale_.x_ = 0;
		keyUI->transform_.localScale_.x_ = 0;
	}
}

void Goal::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
	if (sceneTransitionCount == MaxSceneTransitionCount && tempLiv != nullptr && tempLiv->GetTeam() == team)
	{
		sceneTransitionCount--;
		return;
	}
}