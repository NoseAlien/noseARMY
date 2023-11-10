#include "TitleScene.h"

#include "SceneTransition.h"

void TitleScene::Initialize()
{
	//画像
	titleImg = ADXImage::LoadADXImage("texture/noseARMY_logo.png");
	backGroundTex = ADXImage::LoadADXImage("texture/skyBG.png");
	keyImg = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");

	//マテリアル
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect = ADXModel::CreateRect();

	//オブジェクト

	ADXObject* temp = ADXObject::Create();
	shutter_ = temp->AddComponent<SceneTransition>();

	temp = ADXObject::Create({ 0,0,-2 });
	camera_ = temp->AddComponent<ADXCamera>();	camera_->GetGameObject()->transform_.localPosition_ = { 0,0,-2 };

	title = ADXObject::Create();
	title->transform_.UpdateMatrix();
	title->texture_ = titleImg;
	title->model_ = &rect;
	title->material_ = unlitMat;
	title->renderLayer_ = 1;

	key = ADXObject::Duplicate(*title);
	key->transform_.localScale_ = { 0.5,0.5,0.5 };
	key->texture_ = keyImg;
	key->renderLayer_ = 1;

	backGround_ = ADXObject::Create();
	backGround_->transform_.rectTransform_ = true;
	backGround_->transform_.UpdateMatrix();
	backGround_->model_ = &rect;
	backGround_->texture_ = backGroundTex;
	backGround_->renderLayer_ = -1;
}

void TitleScene::Update()
{
	title->transform_.localPosition_.y_ = sin(clock() * 0.001f) * 0.01f + 0.2f;
	key->transform_.localPosition_.y_ = sin(clock() * 0.001f - 1) * 0.01f - 0.4f;
	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
	{
		SceneTransition::ChangeScene(2);
	}
}