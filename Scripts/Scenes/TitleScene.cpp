#include "TitleScene.h"
#include "SceneTransition.h"
#include "ADXKeyConfig.h"
#include "ADXTextRenderer.h"
#include "ADXModelRenderer.h"

void TitleScene::Initialize()
{
	//画像
	titleImg_ = ADXImage::LoadADXImage("texture/noseARMY_logo.png");
	backGroundTex_ = ADXImage::LoadADXImage("texture/skyBG.png");
	keyImg_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");

	//マテリアル
	unlitMat_ = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect_ = ADXModel::CreateRect();

	//オブジェクト

	ADXObject* temp = ADXObject::Create();
	shutter_ = temp->AddComponent<SceneTransition>();

	temp = ADXObject::Create({ 0,0,-2 });
	camera_ = temp->AddComponent<ADXCamera>();	camera_->GetGameObject()->transform_.localPosition_ = { 0,0,-2 };

	title_ = ADXObject::Create();
	ADXModelRenderer* tempRenderer = title_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = titleImg_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	title_->renderLayer_ = 1;

	key_ = ADXObject::Duplicate(*title_);
	key_->transform_.localScale_ = { 0.5,0.5,0.5 };
	tempRenderer = key_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = keyImg_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	key_->renderLayer_ = 1;

	backGround_ = ADXObject::Create();
	backGround_->transform_.rectTransform_ = true;
	tempRenderer = backGround_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = backGroundTex_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	backGround_->renderLayer_ = -1;

	copyright_ = ADXObject::Create();
	copyright_->transform_.rectTransform_ = true;
	copyright_->renderLayer_ = 1;
	copyright_->AddComponent<ADXTextRenderer>();
	copyright_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	copyright_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	copyright_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	copyright_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::lowerCenter;
	copyright_->GetComponent<ADXTextRenderer>()->text_ = "2024 Terapeta";
	copyright_->transform_.localPosition_ = { 0,-0.9f,0 };
	copyright_->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	copyright_->transform_.localScale_ *= 0.05f;

	version_ = ADXObject::Create();
	version_->transform_.rectTransform_ = true;
	version_->renderLayer_ = 1;
	version_->AddComponent<ADXTextRenderer>();
	version_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	version_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	version_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	version_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::lowerRight;
	version_->GetComponent<ADXTextRenderer>()->text_ = "ver 0.3.1";
	version_->transform_.localPosition_ = { 0.9f,-0.9f,0 };
	version_->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	version_->transform_.localScale_ *= 0.05f;
}

void TitleScene::Update()
{
	title_->transform_.localPosition_.y_ = sin(clock() * 0.001f) * 0.01f + 0.2f;
	key_->transform_.localPosition_.y_ = sin(clock() * 0.001f - 1) * 0.01f - 0.4f;
	if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("select")
		|| ADXKeyConfig::GetCurrentInstance()->GetInputDown("back"))
	{
		SceneTransition::ChangeScene(2);
	}
}