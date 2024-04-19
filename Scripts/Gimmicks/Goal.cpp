#include "Gimmicks/Goal.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXKeyConfig.h"

const float uiShakeSpeed = 0.001f;
const float uiShakeScale = 0.02f;
const float clearUIScale = 0.7f;
const float keyUIScale = 0.45f;
const float uiExpandSpeed = 0.3f;
const ADXVector3 clearUIPos = { 0,0.5f,0 };
const ADXVector3 keyUIPos = { 0,-0.5f,0 };

void Goal::Initialize(const std::string& setTeam)
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = ADXCollider::box;

	team_ = setTeam;
	sceneTransitionCount_ = MaxSceneTransitionCount;

	rectModel_ = ADXModel::CreateRect();
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");

	ADXModelRenderer* tempRenderer = GetGameObject()->AddComponent<ADXModelRenderer>();
	GetGameObject()->sortingOrder_ = 3;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/goalField.png");
	tempRenderer->model_ = &boxModel_;

	clearUI_ = ADXObject::Create();
	clearUI_->transform_.rectTransform_ = true;
	tempRenderer = clearUI_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rectModel_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/clear.png");
	clearUI_->renderLayer_ = 5;

	keyUI_ = ADXObject::Duplicate(*clearUI_);
	tempRenderer = keyUI_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rectModel_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
}

void Goal::UniqueUpdate()
{
	//「CLEAR」の文字を上下に揺らす
	clearUI_->transform_.localPosition_ = clearUIPos;
	clearUI_->transform_.localPosition_.y_ += sin(clock() * uiShakeSpeed) * uiShakeScale;
	clearUI_->transform_.localScale_ = { clearUI_->transform_.localScale_.x_,clearUIScale,1 };
	//「PRESS [SPACE]」の文字を上下に揺らす
	keyUI_->transform_.localPosition_ = keyUIPos;
	keyUI_->transform_.localPosition_.y_ += sin(clock() * uiShakeSpeed) * uiShakeScale;
	keyUI_->transform_.localScale_ = { keyUI_->transform_.localScale_.x_,keyUIScale,1 };

	if (sceneTransitionCount_ != MaxSceneTransitionCount)
	{
		//「CLEAR」の文字を表示
		clearUI_->transform_.localScale_.x_ += (clearUIScale / ADXWindow::GetInstance()->GetAspect() - clearUI_->transform_.localScale_.x_) * uiExpandSpeed;

		sceneTransitionCount_--;
		if (sceneTransitionCount_ <= 0)
		{
			//「PRESS [SPACE]」の文字を表示
			keyUI_->transform_.localScale_.x_ += (keyUIScale / ADXWindow::GetInstance()->GetAspect() - keyUI_->transform_.localScale_.x_) * uiExpandSpeed;
			if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("select")
				|| ADXKeyConfig::GetCurrentInstance()->GetInputDown("back"))
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
		//文字を隠す
		clearUI_->transform_.localScale_.x_ = 0;
		keyUI_->transform_.localScale_.x_ = 0;
	}
}

void Goal::OnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	//触れたらクリア演出
	LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
	if (sceneTransitionCount_ == MaxSceneTransitionCount && tempLiv != nullptr && tempLiv->GetTeam() == team_)
	{
		sceneTransitionCount_--;
		return;
	}
}