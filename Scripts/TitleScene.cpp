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
	camera_ = temp->AddComponent<ADXCamera>();	camera_->GetGameObject()->transform.localPosition_ = { 0,0,-2 };

	title = ADXObject::Create();
	title->transform.UpdateMatrix();
	title->texture = titleImg;
	title->model = &rect;
	title->material = unlitMat;
	title->renderLayer = 1;

	key = ADXObject::Duplicate(*title);
	key->transform.localScale_ = { 0.5,0.5,0.5 };
	key->texture = keyImg;
	key->renderLayer = 1;

	backGround_ = ADXObject::Create();
	backGround_->transform.rectTransform = true;
	backGround_->transform.UpdateMatrix();
	backGround_->model = &rect;
	backGround_->texture = backGroundTex;
	backGround_->renderLayer = -1;
}

void TitleScene::Update()
{
	title->transform.localPosition_.y = sin(clock() * 0.001f) * 0.01f + 0.2f;
	key->transform.localPosition_.y = sin(clock() * 0.001f - 1) * 0.01f - 0.4f;
	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
	{
		SceneTransition::ChangeScene(2);
	}
}