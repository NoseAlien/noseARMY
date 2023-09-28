#include "ADXScene.h"
#include "ADXSceneManager.h"
#include "ADXKeyBoardInput.h"
#include "ADXUtility.h"
#include <time.h>

ADXScene::ADXScene()
{

}

void ADXScene::Initialize()
{
	startTime = 0;

	//画像
	titleImg = ADXImage::LoadADXImage("ADXEngine_logo.png");
	logoImg_A = ADXImage::LoadADXImage("logo_A->png");
	logoImg_D = ADXImage::LoadADXImage("logo_D->png");
	logoImg_X = ADXImage::LoadADXImage("logo_X->png");
	logoImg_Engine = ADXImage::LoadADXImage("logo_Engine->png");

	logoJingle = ADXAudio::LoadADXAudio("sound/ADXJingle.wav");

	//マテリアル
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect = ADXModel::CreateRect();

	//オブジェクト

	ADXObject* temp = ADXObject::Create({0,0,-2});
	camera_ = temp->AddComponent<ADXCamera>();

	title = ADXObject::Create({ 0,0,0.1f },ADXQuaternion::IdentityQuaternion());
	title->transform.UpdateMatrix();
	title->texture = titleImg;
	title->model = &rect;
	title->material = unlitMat;

	logo_A = ADXObject::Duplicate(*title);
	logo_A->transform.localPosition_ = { 0,0,0 };
	logo_A->transform.localScale_ = { 1,0.5f,1 };
	logo_A->texture = logoImg_A;

	logo_D = ADXObject::Duplicate(*logo_A);
	logo_D->texture = logoImg_D;

	logo_X = ADXObject::Duplicate(*logo_A);
	logo_X->texture = logoImg_X;

	logo_Engine = ADXObject::Duplicate(*logo_A);
	logo_Engine->texture = logoImg_Engine;


	startTime = clock();

	logoJingle.Play();
}

void ADXScene::Update()
{
	title->material.alpha = (float)(clock() - startTime) * 0.0004f;

	logo_A->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 2900, 3700, 0, 1);
	logo_A->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 2900, 3700, 0.2f, 0), 0), 2);

	logo_D->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3200, 4000, 0, 1);
	logo_D->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3200, 4000, 0.2f, 0), 0), 2);

	logo_X->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3500, 4300, 0, 1);
	logo_X->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3500, 4300, 0.2f, 0), 0), 2);

	logo_Engine->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3800, 4600, 0, 1);
	logo_Engine->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3800, 4600, 0.2f, 0), 0), 2);

	{
		float rotAngle = (float)abs(pow(abs(min(clock() - startTime - 5200, 0)) * 0.001f, 3.5f));
		title->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });
	}

	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE) || (clock() - startTime >= 7000))
	{
		logoJingle.Stop();
		ADXSceneManager::GetCurrentInstance()->SetSceneNum(1);
	}
	title->material.alpha = (float)(clock() - startTime) * 0.0004f;

	logo_A->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 2900, 3700, 0, 1);
	logo_A->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 2900, 3700, 0.2f, 0), 0), 2);

	logo_D->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3200, 4000, 0, 1);
	logo_D->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3200, 4000, 0.2f, 0), 0), 2);

	logo_X->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3500, 4300, 0, 1);
	logo_X->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3500, 4300, 0.2f, 0), 0), 2);

	logo_Engine->material.alpha = ADXUtility::ValueMapping((float)(clock() - startTime), 3800, 4600, 0, 1);
	logo_Engine->transform.localPosition_.y = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime), 3800, 4600, 0.2f, 0), 0), 2);

	{
		float rotAngle = (float)abs(pow(abs(min(clock() - startTime - 5200, 0)) * 0.001f, 3.5f));
		title->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });
	}

	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE) || (clock() - startTime >= 7000))
	{
		logoJingle.Stop();
		ADXSceneManager::GetCurrentInstance()->SetSceneNum(1);
	}
}