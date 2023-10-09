#include "SceneGate.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXCollider.h"

void SceneGate::Initialize(std::string setTeam)
{
	GetGameObject()->texture = ADXImage::LoadADXImage("goalField.png");
	GetGameObject()->sortingOrder = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;

	team = setTeam;

	rectModel = ADXModel::CreateRect();
	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model = &boxModel;

	keyUI = ADXObject::Create();
	keyUI->transform.rectTransform = true;
	keyUI->model = &rectModel;
	keyUI->texture = ADXImage::LoadADXImage("PRESS_SPACE.png");
	keyUI->renderLayer = 5;
}

void SceneGate::UniqueUpdate()
{
	keyUI->transform.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI->transform.localScale_ = { keyUI->transform.localScale_.x,0.45f,1 };

	if (hitted)
	{
		keyUI->transform.localScale_.x += (0.45f / ADXWindow::GetAspect() - keyUI->transform.localScale_.x) * 0.3f;
		if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			SceneTransition::ChangeScene(3);
		}

		hitted = false;
	}
	else
	{
		keyUI->transform.localScale_.x = 0;
	}
}

void SceneGate::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	for (auto& objItr : LiveEntity::GetLiveEntities())
	{
		if (!objItr->GetGameObject()->GetComponents<ADXCollider>().empty() && col == objItr->GetGameObject()->GetComponent<ADXCollider>() && objItr->GetTeam() == team)
		{
			hitted = true;
			return;
		}
	}
}