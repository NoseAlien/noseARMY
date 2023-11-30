#include "SceneGate.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXCollider.h"

void SceneGate::Initialize(const std::string& setTeam)
{
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/goalField.dds");
	GetGameObject()->sortingOrder_ = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = box;

	team_ = setTeam;

	rectModel_ = ADXModel::CreateRect();
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model_ = &boxModel_;

	keyUI_ = ADXObject::Create();
	keyUI_->transform_.rectTransform_ = true;
	keyUI_->model_ = &rectModel_;
	keyUI_->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.dds");
	keyUI_->renderLayer_ = 5;
}

void SceneGate::UniqueUpdate()
{
	keyUI_->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI_->transform_.localScale_ = { keyUI_->transform_.localScale_.x_,0.45f,1 };

	if (hitted_)
	{
		keyUI_->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI_->transform_.localScale_.x_) * 0.3f;
		if (ADXKeyBoardInput::GetCurrentInstance()->GetKeyDown(DIK_SPACE))
		{
			SceneTransition::ChangeScene(3);
		}

		hitted_ = false;
	}
	else
	{
		keyUI_->transform_.localScale_.x_ = 0;
	}
}

void SceneGate::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<LiveEntity>() != nullptr
		&& col->GetGameObject()->GetComponent<LiveEntity>()->GetTeam() == team_)
	{
		hitted_ = true;
		return;
	}
}