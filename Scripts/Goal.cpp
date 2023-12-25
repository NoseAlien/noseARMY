#include "Goal.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXKeyConfig.h"

void Goal::Initialize(const std::string& setTeam)
{
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/goalField.png");
	GetGameObject()->sortingOrder_ = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = box;

	team_ = setTeam;
	sceneTransitionCount_ = MaxSceneTransitionCount;

	rectModel_ = ADXModel::CreateRect();
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model_ = &boxModel_;

	clearUI_ = ADXObject::Create();
	clearUI_->transform_.rectTransform_ = true;
	clearUI_->model_ = &rectModel_;
	clearUI_->texture_ = ADXImage::LoadADXImage("texture/clear.png");
	clearUI_->renderLayer_ = 5;

	keyUI_ = ADXObject::Duplicate(*clearUI_);
	keyUI_->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
}

void Goal::UniqueUpdate()
{
	clearUI_->transform_.localPosition_ = { 0,0.5f + sin(clock() * 0.001f) * 0.02f,0 };
	clearUI_->transform_.localScale_ = { clearUI_->transform_.localScale_.x_,0.7f,1 };

	keyUI_->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI_->transform_.localScale_ = { keyUI_->transform_.localScale_.x_,0.45f,1 };

	if (sceneTransitionCount_ != MaxSceneTransitionCount)
	{
		clearUI_->transform_.localScale_.x_ += (0.7f / ADXWindow::GetAspect() - clearUI_->transform_.localScale_.x_) * 0.3f;
		clearUI_->transform_.UpdateMatrix();

		sceneTransitionCount_--;
		if (sceneTransitionCount_ <= 0)
		{
			keyUI_->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI_->transform_.localScale_.x_) * 0.3f;
			if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("select"))
			{
				SceneTransition::ChangeScene(2);
			}
		}
		else
		{
			keyUI_->transform_.localScale_.x_ = 0;
		}
	}
	else
	{
		clearUI_->transform_.localScale_.x_ = 0;
		keyUI_->transform_.localScale_.x_ = 0;
	}
}

void Goal::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
	if (sceneTransitionCount_ == MaxSceneTransitionCount && tempLiv != nullptr && tempLiv->GetTeam() == team_)
	{
		sceneTransitionCount_--;
		return;
	}
}