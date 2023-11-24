#include "ADXScene.h"
#include "ADXSceneManager.h"
#include "ADXUtility.h"
#include <time.h>

ADXScene::ADXScene()
{

}

void ADXScene::Initialize()
{
	startTime_ = 0;

	//画像
	titleImg_ = ADXImage::LoadADXImage("texture/ADXEngine_logo.png");
	logoImg_A_ = ADXImage::LoadADXImage("texture/logo_A.png");
	logoImg_D_ = ADXImage::LoadADXImage("texture/logo_D.png");
	logoImg_X_ = ADXImage::LoadADXImage("texture/logo_X.png");
	logoImg_Engine_ = ADXImage::LoadADXImage("texture/logo_Engine.png");

	logoJingle_ = ADXAudio::LoadADXAudio("sound/ADXJingle.wav");

	//マテリアル
	unlitMat_ = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect_ = ADXModel::CreateRect();

	//オブジェクト

	ADXObject* temp = ADXObject::Create({0,0,-2});
	camera_ = temp->AddComponent<ADXCamera>();

	title_ = ADXObject::Create({ 0,0,0.1f },ADXQuaternion::IdentityQuaternion());
	title_->transform_.UpdateMatrix();
	title_->texture_ = titleImg_;
	title_->model_ = &rect_;
	title_->material_ = unlitMat_;
	title_->material_.alpha_ = 0;

	logo_A_ = ADXObject::Duplicate(*title_);
	logo_A_->transform_.localPosition_ = { 0,0,0 };
	logo_A_->transform_.localScale_ = { 1,0.5f,1 };
	logo_A_->texture_ = logoImg_A_;

	logo_D_ = ADXObject::Duplicate(*logo_A_);
	logo_D_->texture_ = logoImg_D_;

	logo_X_ = ADXObject::Duplicate(*logo_A_);
	logo_X_->texture_ = logoImg_X_;

	logo_Engine_ = ADXObject::Duplicate(*logo_A_);
	logo_Engine_->texture_ = logoImg_Engine_;


	startTime_ = clock();

	logoJingle_.Play();
}

void ADXScene::Update()
{
	title_->material_.alpha_ = (float)(clock() - startTime_) * 0.0004f;

	logo_A_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 2900, 3700, 0, 1);
	logo_A_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 2900, 3700, 0.2f, 0), 0), 2);

	logo_D_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3200, 4000, 0, 1);
	logo_D_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3200, 4000, 0.2f, 0), 0), 2);

	logo_X_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3500, 4300, 0, 1);
	logo_X_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3500, 4300, 0.2f, 0), 0), 2);

	logo_Engine_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3800, 4600, 0, 1);
	logo_Engine_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3800, 4600, 0.2f, 0), 0), 2);

	{
		float rotAngle = (float)abs(pow(abs(min(clock() - startTime_ - 5200, 0)) * 0.001f, 3.5f));
		title_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });
	}

	if ((clock() - startTime_ >= 7000)
		|| ADXKeyBoardInput::GetCurrentInstance()->GetKeyDown(DIK_SPACE)
		|| (ADXGamePadInput::GetCurrentInstance() != nullptr
		&& (ADXGamePadInput::GetCurrentInstance()->GetButtonDown(A)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(B)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(X)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(Y)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(START)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(BACK)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(LB)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(RB)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(LT)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(RT))))
	{
		logoJingle_.Stop();
		ADXSceneManager::GetCurrentInstance()->SetSceneNum(1);
	}
	title_->material_.alpha_ = (float)(clock() - startTime_) * 0.0004f;

	logo_A_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 2900, 3700, 0, 1);
	logo_A_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 2900, 3700, 0.2f, 0), 0), 2);

	logo_D_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3200, 4000, 0, 1);
	logo_D_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3200, 4000, 0.2f, 0), 0), 2);

	logo_X_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3500, 4300, 0, 1);
	logo_X_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3500, 4300, 0.2f, 0), 0), 2);

	logo_Engine_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3800, 4600, 0, 1);
	logo_Engine_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3800, 4600, 0.2f, 0), 0), 2);

	{
		float rotAngle = (float)abs(pow(abs(min(clock() - startTime_ - 5200, 0)) * 0.001f, 3.5f));
		title_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });
	}

	if (ADXKeyBoardInput::GetCurrentInstance()->GetKeyDown(DIK_SPACE) || (clock() - startTime_ >= 7000))
	{
		logoJingle_.Stop();
		ADXSceneManager::GetCurrentInstance()->SetSceneNum(1);
	}
}