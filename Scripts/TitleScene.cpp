#include "TitleScene.h"

#include "SceneTransition.h"

TitleScene::TitleScene()
{
}

void TitleScene::Initialize()
{
	//画像
	titleImg = ADXImage::LoadADXImage("noseARMY_logo.png");
	keyImg = ADXImage::LoadADXImage("PRESS_SPACE.png");

	//マテリアル
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect = ADXModel::CreateRect();

	//オブジェクト

	camera_.ADXObject::Initialize();
	camera_.transform.localPosition_ = { 0,0,-2 };
	camera_.Initialize();

	title.Initialize();
	title.transform.UpdateMatrix();
	title.texture = titleImg;
	title.model = &rect;
	title.material = unlitMat;

	key = ADXObject::Duplicate(title);
	key.transform.localScale_ = { 0.5,0.5,0.5 };
	key.texture = keyImg;


	objs.push_back(&camera_);
	objs.push_back(&title);
	objs.push_back(&key);
}

void TitleScene::Update()
{
	title.transform.localPosition_.y = sin(clock() * 0.001f) * 0.01f + 0.2f;
	key.transform.localPosition_.y = sin(clock() * 0.001f - 1) * 0.01f - 0.4f;
	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
	{
		SceneTransition::ChangeScene(2);
	}

	for (auto& itr : objs)
	{
		itr->Update();
	}
}