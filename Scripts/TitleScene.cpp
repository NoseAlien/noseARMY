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
	title_->transform_.UpdateMatrix();
	title_->GetComponent<ADXModelRenderer>()->texture_ = titleImg_;
	title_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	title_->GetComponent<ADXModelRenderer>()->material_ = unlitMat_;
	title_->renderLayer_ = 1;

	key_ = ADXObject::Duplicate(*title_);
	key_->transform_.localScale_ = { 0.5,0.5,0.5 };
	key_->GetComponent<ADXModelRenderer>()->texture_ = keyImg_;
	key_->renderLayer_ = 1;

	backGround_ = ADXObject::Create();
	backGround_->transform_.rectTransform_ = true;
	backGround_->transform_.UpdateMatrix();
	backGround_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	backGround_->GetComponent<ADXModelRenderer>()->texture_ = backGroundTex_;
	backGround_->renderLayer_ = -1;

	copyright_ = ADXObject::Create();
	copyright_->transform_.rectTransform_ = true;
	copyright_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	copyright_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
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
	version_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	version_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
	version_->renderLayer_ = 1;
	version_->AddComponent<ADXTextRenderer>();
	version_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	version_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	version_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	version_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::lowerRight;
	version_->GetComponent<ADXTextRenderer>()->text_ = "ver 0.2";
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