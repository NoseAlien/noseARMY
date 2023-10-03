#include "TitleScene.h"

#include "SceneTransition.h"

void TitleScene::Initialize()
{
	//画像
	titleImg = ADXImage::LoadADXImage("noseARMY_logo.png");
	keyImg = ADXImage::LoadADXImage("PRESS_SPACE.png");

	//マテリアル
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect = ADXModel::CreateRect();

	//オブジェクト

	ADXObject* temp = ADXObject::Create({ 0,0,-2 });
	camera_ = temp->AddComponent<ADXCamera>();
	camera_->GetGameObject()->transform.localPosition_ = { 0,0,-2 };
	camera_->Initialize();

	title = ADXObject::Create();
	title->transform.UpdateMatrix();
	title->texture = titleImg;
	title->model = &rect;
	title->material = unlitMat;

	key = ADXObject::Duplicate(*title);
	key->transform.localScale_ = { 0.5,0.5,0.5 };
	key->texture = keyImg;
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