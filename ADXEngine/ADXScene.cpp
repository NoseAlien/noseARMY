#include "ADXScene.h"
#include "ADXSceneManager.h"
#include "ADXUtility.h"
#include "ADXModelRenderer.h"
#include <time.h>

const int32_t logoSceneTimeFrame = 7000;
const int32_t titleSceneIndex = 1;
const int32_t logoAnimationPow = 2;
const float logoTextAspect = 0.5f;
const float logoStartHeight = 0.2f;
const float logoAlphaSpeed = 0.0004f;
const float logoRotateSpeed = 0.001f;
const float logoRotatePow = 3.5f;
const int32_t logoRotateTime = 5200;
const ADXVector2 logo_A_displayTime = { 2900,3700 };
const ADXVector2 logo_D_displayTime = { 3200,4000 };
const ADXVector2 logo_X_displayTime = { 3500,4300 };
const ADXVector2 logo_Engine_displayTime = { 3800,4600 };
const ADXVector3 cameraPos = { 0,0,-2 };
const ADXVector3 logoTextPos = { 0,0,-0.1f };

void ADXScene::Initialize()
{
	startTime_ = 0;

	//画像
	titleImg_ = ADXImage::LoadADXImage("texture/ADXEngine_logo.png");
	logoImg_A_ = ADXImage::LoadADXImage("texture/logo_A.png");
	logoImg_D_ = ADXImage::LoadADXImage("texture/logo_D.png");
	logoImg_X_ = ADXImage::LoadADXImage("texture/logo_X.png");
	logoImg_Engine_ = ADXImage::LoadADXImage("texture/logo_Engine.png");

	//マテリアル
	unlitMat_ = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect_ = ADXModel::CreateRect();

	//オブジェクト

	ADXObject* temp = ADXObject::Create(cameraPos);
	camera_ = temp->AddComponent<ADXCamera>();

	title_ = ADXObject::Create();
	title_->transform_.UpdateMatrix();
	ADXModelRenderer* tempRenderer = title_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = titleImg_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	tempRenderer->material_.alpha_ = 0;
	logoJingle_ = title_->AddComponent<ADXAudioSource>();
	logoJingle_->LoadADXAudio("sound/ADXJingle.wav");

	logo_A_ = ADXObject::Duplicate(*title_);
	logo_A_->transform_.localPosition_ = logoTextPos;
	logo_A_->transform_.localScale_ = { 1,logoTextAspect,1 };
	tempRenderer = logo_A_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = logoImg_A_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	tempRenderer->material_.alpha_ = 0;

	logo_D_ = ADXObject::Duplicate(*logo_A_);
	tempRenderer = logo_D_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = logoImg_D_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	tempRenderer->material_.alpha_ = 0;

	logo_X_ = ADXObject::Duplicate(*logo_A_);
	tempRenderer = logo_X_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = logoImg_X_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	tempRenderer->material_.alpha_ = 0;

	logo_Engine_ = ADXObject::Duplicate(*logo_A_);
	tempRenderer = logo_Engine_->AddComponent<ADXModelRenderer>();
	tempRenderer->texture_ = logoImg_Engine_;
	tempRenderer->model_ = &rect_;
	tempRenderer->material_ = unlitMat_;
	tempRenderer->material_.alpha_ = 0;


	startTime_ = clock();

	//ロゴ画面のジングルを再生
	logoJingle_->Play();
}

void ADXScene::Update()
{
	//エンジンロゴの見た目が時間と共に変化していく
	title_->GetComponent<ADXModelRenderer>()->material_.alpha_ = (float)(clock() - startTime_) * logoAlphaSpeed;

	logo_A_->GetComponent<ADXModelRenderer>()->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_A_displayTime.x_, logo_A_displayTime.y_, 0, 1);
	logo_A_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_A_displayTime.x_, logo_A_displayTime.y_, logoStartHeight, 0), 0), logoAnimationPow);

	logo_D_->GetComponent<ADXModelRenderer>()->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_D_displayTime.x_, logo_D_displayTime.y_, 0, 1);
	logo_D_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_D_displayTime.x_, logo_D_displayTime.y_, logoStartHeight, 0), 0), logoAnimationPow);

	logo_X_->GetComponent<ADXModelRenderer>()->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_X_displayTime.x_, logo_X_displayTime.y_, 0, 1);
	logo_X_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_X_displayTime.x_, logo_X_displayTime.y_, logoStartHeight, 0), 0), logoAnimationPow);

	logo_Engine_->GetComponent<ADXModelRenderer>()->material_.alpha_ = ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_Engine_displayTime.x_, logo_Engine_displayTime.y_, 0, 1);
	logo_Engine_->transform_.localPosition_.y_ = (float)pow(max(ADXUtility::ValueMapping((float)(clock() - startTime_),
		logo_Engine_displayTime.x_, logo_Engine_displayTime.y_, logoStartHeight, 0), 0), logoAnimationPow);

	float rotAngle = (float)abs(pow(abs(min(clock() - startTime_ - logoRotateTime, 0)) * logoRotateSpeed, logoRotatePow));
	title_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0, 0, rotAngle });

	//何かボタンが押されたらスキップ
	if ((clock() - startTime_ >= logoSceneTimeFrame)
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
		logoJingle_->Stop();
		ADXSceneManager::GetCurrentInstance()->SetSceneIndex(titleSceneIndex);
	}
}