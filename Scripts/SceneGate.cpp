#include "SceneGate.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXCollider.h"

void SceneGate::Initialize(const std::string& setTeam)
{
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/goalField.png");
	GetGameObject()->sortingOrder_ = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = box;

	team = setTeam;

	rectModel = ADXModel::CreateRect();
	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model_ = &boxModel;

	keyUI = ADXObject::Create();
	keyUI->transform_.rectTransform_ = true;
	keyUI->model_ = &rectModel;
	keyUI->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
	keyUI->renderLayer_ = 5;
}

void SceneGate::UniqueUpdate()
{
	keyUI->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI->transform_.localScale_ = { keyUI->transform_.localScale_.x_,0.45f,1 };

	if (hitted)
	{
		keyUI->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI->transform_.localScale_.x_) * 0.3f;
		if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			SceneTransition::ChangeScene(3);
		}

		hitted = false;
	}
	else
	{
		keyUI->transform_.localScale_.x_ = 0;
	}
}

void SceneGate::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<LiveEntity>() != nullptr
		&& col->GetGameObject()->GetComponent<LiveEntity>()->GetTeam() == team)
	{
		hitted = true;
		return;
	}
}