#include "Goal.h"
#include "LiveEntity.h"
#include "SceneTransition.h"

void Goal::Initialize(std::string setTeam)
{
	GetGameObject()->texture = ADXImage::LoadADXImage("goalField.png");
	GetGameObject()->sortingOrder = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;

	team = setTeam;
	sceneTransitionCount = MaxSceneTransitionCount;

	rectModel = ADXModel::CreateRect();
	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model = &boxModel;

	clearUI = ADXObject::Create();
	clearUI->transform.rectTransform = true;
	clearUI->model = &rectModel;
	clearUI->texture = ADXImage::LoadADXImage("clear.png");
	clearUI->renderLayer = 5;

	keyUI = ADXObject::Duplicate(*clearUI);
	keyUI->texture = ADXImage::LoadADXImage("PRESS_SPACE.png");
}

void Goal::UniqueUpdate()
{
	clearUI->transform.localPosition_ = { 0,0.5f + sin(clock() * 0.001f) * 0.02f,0 };
	clearUI->transform.localScale_ = { clearUI->transform.localScale_.x,0.7f,1 };

	keyUI->transform.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI->transform.localScale_ = { keyUI->transform.localScale_.x,0.45f,1 };

	if (sceneTransitionCount != MaxSceneTransitionCount)
	{
		clearUI->transform.localScale_.x += (0.7f / ADXWindow::GetAspect() - clearUI->transform.localScale_.x) * 0.3f;
		clearUI->transform.UpdateMatrix();

		sceneTransitionCount--;
		if (sceneTransitionCount <= 0)
		{
			keyUI->transform.localScale_.x += (0.45f / ADXWindow::GetAspect() - keyUI->transform.localScale_.x) * 0.3f;
			if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
			{
				SceneTransition::ChangeScene(2);
			}
		}
		else
		{
			keyUI->transform.localScale_.x = 0;
		}
	}
	else
	{
		clearUI->transform.localScale_.x = 0;
		keyUI->transform.localScale_.x = 0;
	}
}

void Goal::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	if (sceneTransitionCount == MaxSceneTransitionCount)
	{
		for (auto& objItr : LiveEntity::GetLiveEntities())
		{
			if (!objItr->GetGameObject()->GetComponents<ADXCollider>().empty() && col == objItr->GetGameObject()->GetComponent<ADXCollider>() && objItr->GetTeam() == team)
			{
				sceneTransitionCount--;
				return;
			}
		}
	}
}