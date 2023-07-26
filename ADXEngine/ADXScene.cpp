#include "ADXScene.h"
#include "ADXSceneManager.h"
#include "ADXUtility.h"
#include <time.h>

ADXScene::ADXScene()
{

}

void ADXScene::Initialize()
{
	startTime = 0;

	switch (ADXSceneManager::GetSceneNum())
	{
	case 0:
		//画像
		titleImg = ADXImage::LoadADXImage("ADXEngine_logo.png");
		logoImg_A = ADXImage::LoadADXImage("logo_A.png");
		logoImg_D = ADXImage::LoadADXImage("logo_D.png");
		logoImg_X = ADXImage::LoadADXImage("logo_X.png");
		logoImg_Engine = ADXImage::LoadADXImage("logo_Engine.png");

		//マテリアル
		unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

		rect = ADXModel::CreateRect();

		//オブジェクト

		camera_.ADXObject::Initialize();
		camera_.transform.localPosition_ = { 0,0,-2 };
		camera_.Initialize();

		title.Initialize();
		title.transform.localPosition_ = { 0,0,0.1f };
		title.transform.localScale_ = { 1,1,1 };
		title.transform.UpdateMatrix();
		title.texture = titleImg;
		title.model = &rect;
		title.material = unlitMat;

		logo_A = ADXObject::Duplicate(title);
		logo_A.transform.localPosition_ = { 0,0,0 };
		logo_A.transform.localScale_ = { 1,0.5f,1 };
		logo_A.texture = logoImg_A;

		logo_D = ADXObject::Duplicate(logo_A);
		logo_D.texture = logoImg_D;

		logo_X = ADXObject::Duplicate(logo_A);
		logo_X.texture = logoImg_X;

		logo_Engine = ADXObject::Duplicate(logo_A);
		logo_Engine.texture = logoImg_Engine;


		objs.push_back(&camera_);
		objs.push_back(&title);
		objs.push_back(&logo_A);
		objs.push_back(&logo_D);
		objs.push_back(&logo_X);
		objs.push_back(&logo_Engine);

		startTime = clock();

		break;

	case 1:
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
		break;
	}
}

void ADXScene::Update()
{
	switch (ADXSceneManager::GetSceneNum())
	{
	case 0:
		title.material.alpha = (float)(clock() - startTime) * 0.0004f;

		logo_A.material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime),2600,3400,0,1);
		logo_A.transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 2600, 3400, 0.2f, 0), 0), 2);

		logo_D.material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 2900, 3700, 0, 1);
		logo_D.transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 2900, 3700, 0.2f, 0), 0), 2);

		logo_X.material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3200, 4000, 0, 1);
		logo_X.transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3200, 4000, 0.2f, 0), 0), 2);

		logo_Engine.material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3500, 4300, 0, 1);
		logo_Engine.transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3500, 4300, 0.2f, 0), 0), 2);

		{
			float rotAngle = (float)abs(pow(abs(min(clock() - startTime - 4800, 0)) * 0.001f, 3.5f));
			title.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });
		}

		if (ADXSceneManager::GetKeyboardInput()->KeyTrigger(DIK_SPACE) || (clock() - startTime >=6500))
		{
			ADXSceneManager::SetSceneNum(1);
		}
		break;
	case 1:
		title.transform.localPosition_.y = sin(clock() * 0.001f) * 0.01f + 0.2f;
		key.transform.localPosition_.y = sin(clock() * 0.001f - 1) * 0.01f - 0.4f;
		if (ADXSceneManager::GetKeyboardInput()->KeyTrigger(DIK_SPACE))
		{
			ADXSceneManager::SetSceneNum(2);
		}
		break;
	}

	for (auto& itr : objs)
	{
		itr->Update();
	}
}