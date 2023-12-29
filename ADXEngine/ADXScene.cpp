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

	//ロゴ画面のジングルを再生
	logoJingle_.Play();
}

void ADXScene::Update()
{
	//エンジンロゴの見た目が時間と共に変化していく
	title_->material_.alpha_ = (float)(clock() - startTime_) * 0.0004f;

	logo_A_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 2900, 3700, 0, 1);
	logo_A_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 2900, 3700, logoStartHeight, 0), 0), logoAnimationPow);

	logo_D_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3200, 4000, 0, 1);
	logo_D_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3200, 4000, logoStartHeight, 0), 0), logoAnimationPow);

	logo_X_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3500, 4300, 0, 1);
	logo_X_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3500, 4300, logoStartHeight, 0), 0), logoAnimationPow);

	logo_Engine_->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_), 3800, 4600, 0, 1);
	logo_Engine_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_), 3800, 4600, logoStartHeight, 0), 0), logoAnimationPow);

	{
		float rotAngle = (float)abs(pow(abs(min(clock() - startTime_ - 5200, 0)) * 0.001f, 3.5f));
		title_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });
	}

	//何かボタンが押されたらスキップ
	if ((clock() - startTime_ >= 7000)
		|| ADXKeyBoardInput::GetCurrentInstance()->GetKeyDown(DIK_SPACE)
		|| (ADXGamePadInput::GetCurrentInstance() != nullptr
		&& (ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::A)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::B)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::X)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::Y)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::START)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::BACK)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::LB)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::RB)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::LT)
		|| ADXGamePadInput::GetCurrentInstance()->GetButtonDown(ADXGamePadInput::RT))))
	{
		logoJingle_.Stop();
		ADXSceneManager::GetCurrentInstance()->SetSceneIndex(titleSceneIndex);
	}
}