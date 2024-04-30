#include "Gimmicks/SceneGate.h"
#include "LiveEntity.h"
#include "SceneTransition.h"
#include "ADXCollider.h"
#include "ADXKeyConfig.h"
#include "ADXTextRenderer.h"

const float stageNameUIScale = 0.5f;

std::string SceneGate::S_nextStageName = "";

void SceneGate::Initialize(const std::string& team, const std::string& stageName)
{
	team_ = team;
	stageName_ = stageName;

	rectModel_ = ADXModel::CreateRect();
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");

	ADXModelRenderer* tempRenderer = GetGameObject()->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/goalField.png");
	tempRenderer->model_ = &boxModel_;
	GetGameObject()->sortingOrder_ = 3;

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = ADXCollider::box;

	keyUI_ = ADXObject::Create();
	keyUI_->transform_.rectTransform_ = true;
	tempRenderer = keyUI_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rectModel_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
	keyUI_->renderLayer_ = 5;
	keyUI_->transform_.localScale_ = { 0,0,0 };

	stageNameUI_ = ADXObject::Create();
	stageNameUI_->transform_.parent_ = &GetGameObject()->transform_;
	stageNameUI_->AddComponent<ADXTextRenderer>();
	stageNameUI_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	stageNameUI_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	stageNameUI_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	stageNameUI_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::middleCenter;
	stageNameUI_->transform_.localPosition_ = { -0.9f,-0.9f,0 };
	stageNameUI_->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	stageNameUI_->transform_.localScale_ *= 0.1f;
}

void SceneGate::UniqueUpdate()
{
	keyUI_->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI_->transform_.localScale_ = { keyUI_->transform_.localScale_.x_,0.45f,1 };

	stageNameUI_->transform_.localPosition_ = { 0,1.1f + stageNameUIScale + sin(clock() * 0.001f - 1) * 0.1f,0 };
	stageNameUI_->transform_.localScale_ = { stageNameUIScale,stageNameUIScale,stageNameUIScale };
	stageNameUI_->GetComponent<ADXTextRenderer>()->text_ = stageName_;

	if (hitted_)
	{
		keyUI_->transform_.localScale_.x_ += (0.45f / ADXWindow::GetInstance()->GetAspect() - keyUI_->transform_.localScale_.x_) * 0.3f;
		if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("select")
			|| ADXKeyConfig::GetCurrentInstance()->GetInputDown("back"))
		{
			S_nextStageName = stageName_;
			SceneTransition::ChangeScene(3);
		}

		hitted_ = false;
	}
	else
	{
		keyUI_->transform_.localScale_.x_ -= keyUI_->transform_.localScale_.x_ * 0.3f;
	}
}

void SceneGate::OnPreRender()
{
	//ステージ名UIをビルボード表示にする
	stageNameUI_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	stageNameUI_->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
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