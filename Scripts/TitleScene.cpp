#include "TitleScene.h"
#include "SceneTransition.h"
#include "ADXKeyConfig.h"

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
	title_->texture_ = titleImg_;
	title_->model_ = &rect_;
	title_->material_ = unlitMat_;
	title_->renderLayer_ = 1;

	key_ = ADXObject::Duplicate(*title_);
	key_->transform_.localScale_ = { 0.5,0.5,0.5 };
	key_->texture_ = keyImg_;
	key_->renderLayer_ = 1;

	backGround_ = ADXObject::Create();
	backGround_->transform_.rectTransform_ = true;
	backGround_->transform_.UpdateMatrix();
	backGround_->model_ = &rect_;
	backGround_->texture_ = backGroundTex_;
	backGround_->renderLayer_ = -1;
}

void TitleScene::Update()
{
	title_->transform_.localPosition_.y_ = sin(clock() * 0.001f) * 0.01f + 0.2f;
	key_->transform_.localPosition_.y_ = sin(clock() * 0.001f - 1) * 0.01f - 0.4f;
	if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("select"))
	{
		SceneTransition::ChangeScene(2);
	}
}