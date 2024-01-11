#include "SceneGate.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXCollider.h"
#include "ADXKeyConfig.h"
#include "ADXTextRenderer.h"

std::string SceneGate::S_nextStageName = "";

void SceneGate::Initialize(const std::string& team, const std::string& stageName)
{
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/goalField.png");
	GetGameObject()->sortingOrder_ = 1;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = ADXCollider::box;

	team_ = team;
	stageName_ = stageName;

	rectModel_ = ADXModel::CreateRect();
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");

	GetGameObject()->model_ = &boxModel_;

	keyUI_ = ADXObject::Create();
	keyUI_->transform_.rectTransform_ = true;
	keyUI_->model_ = &rectModel_;
	keyUI_->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
	keyUI_->renderLayer_ = 5;
	keyUI_->transform_.localScale_ = { 0,0,0 };

	stageNameUI_ = ADXObject::Create();
	stageNameUI_->transform_.rectTransform_ = true;
	stageNameUI_->model_ = &rectModel_;
	stageNameUI_->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
	stageNameUI_->renderLayer_ = 5;
	stageNameUI_->useDefaultDraw_ = false;
	stageNameUI_->AddComponent<ADXTextRenderer>();
	stageNameUI_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	stageNameUI_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	stageNameUI_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	stageNameUI_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::middleCenter;
	stageNameUI_->transform_.localPosition_ = { -0.9f,-0.9f,0 };
	stageNameUI_->transform_.localScale_.x_ /= ADXWindow::GetAspect();
	stageNameUI_->transform_.localScale_ *= 0.1f;
}

void SceneGate::UniqueUpdate()
{
	keyUI_->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI_->transform_.localScale_ = { keyUI_->transform_.localScale_.x_,0.45f,1 };

	stageNameUI_->transform_.localPosition_ = { 0,0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	stageNameUI_->transform_.localScale_ = { stageNameUI_->transform_.localScale_.x_,0.1f,1 };
	stageNameUI_->GetComponent<ADXTextRenderer>()->text_ = stageName_;

	if (hitted_)
	{
		keyUI_->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI_->transform_.localScale_.x_) * 0.3f;
		stageNameUI_->transform_.localScale_.x_ += (0.1f / ADXWindow::GetAspect() - stageNameUI_->transform_.localScale_.x_) * 0.3f;
		if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("select"))
		{
			S_nextStageName = stageName_;
			SceneTransition::ChangeScene(3);
		}

		hitted_ = false;
	}
	else
	{
		keyUI_->transform_.localScale_.x_ -= keyUI_->transform_.localScale_.x_ * 0.3f;
		stageNameUI_->transform_.localScale_.x_ -= stageNameUI_->transform_.localScale_.x_ * 0.3f;
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