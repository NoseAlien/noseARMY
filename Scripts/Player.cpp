﻿#include "Player.h"
#include "SceneTransition.h"
#include "Gimmicks/Goal.h"
#include "ADXUtility.h"
#include "ADXKeyConfig.h"
#include "ADXAudioListener.h"
#include <time.h>

const float uiExtendSpeed = 0.3f;
const float tutorialWindowSize = 0.3f;
const float deathCountUISize = 0.1f;
const float maxCameraTiltVelocity = 0.7f;
const float cameraTiltForce = 0.3f;
const float cameraTiltSpeedY = 0.08f;
const ADXVector2 cameraHeightLimit = { 0,4 };
const float cameraDistance = 17;
const float listenerRadius = 40;
const float idolAnimAmount = 0.05f;
const float walkAnimAmount = 0.1f;
const float jumpAnimVelocity = 0.5f;
const float jumpAnimAmount = 1.5f;
const float aimSpeed = 0.3f;
const int gameOverLayer = 4;
const int gameOverFilterLayer = 3;
const int uiLayer = 5;

bool Player::GetInputStatus(actionsList action)
{
	std::string cellName = "";
	switch (action)
	{
	case jump:
		cellName = "jump";
		break;
	case attack:
		cellName = "attack";
		break;
	}

	ADXKeyConfig* config = ADXKeyConfig::GetCurrentInstance();
	return config != nullptr && config->GetInput(cellName);
}

bool Player::GetInputStatusTrigger(actionsList action)
{
	std::string cellName = "";
	switch (action)
	{
	case jump:
		cellName = "jump";
		break;
	case attack:
		cellName = "attack";
		break;
	}

	ADXKeyConfig* config = ADXKeyConfig::GetCurrentInstance();
	return config != nullptr && config->GetInputDown(cellName);
}

bool Player::GetInputStatusRelease(actionsList action)
{
	std::string cellName = "";
	switch (action)
	{
	case jump:
		cellName = "jump";
		break;
	case attack:
		cellName = "attack";
		break;
	}

	ADXKeyConfig* config = ADXKeyConfig::GetCurrentInstance();
	return config != nullptr && config->GetInputUp(cellName);
}

ADXVector2 Player::GetDirectionInput()
{
	ADXKeyConfig* config = ADXKeyConfig::GetCurrentInstance();
	return config->GetVecInput("vec").Normalize();
}

ADXVector2 Player::GetCameraControlInput()
{
	ADXKeyConfig* config = ADXKeyConfig::GetCurrentInstance();
	return config->GetVecInput("cameraTilt").Normalize();
}

void Player::Move(float walkSpeed, float jumpPower)
{
	ADXVector3 cameraRight = camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 1,0,0 });
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y_ = 0;
	cameraForward = cameraForward.Normalize();

	ADXVector2 inputVec = GetDirectionInput();
	rigidbody_->velocity_ += (cameraRight * inputVec.x_ + cameraForward * inputVec.y_) * walkSpeed;

	float modelScalingTime = (float)clock() * 0.002f;
	float animAmount = idolAnimAmount;

	if (inputVec != ADXVector2{ 0,0 })
	{
		targetRot_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody_->velocity_.x_, rigidbody_->velocity_.z_),0 });

		if (!GetInputStatus(attack))
		{
			modelScalingTime = (float)clock() * 0.015f;
			animAmount = walkAnimAmount;
		}
	}

	visual_->transform_.localScale_ = { 1 + sinf(modelScalingTime) * animAmount,
		(1 + cosf(modelScalingTime) * animAmount) * ADXUtility::ValueMapping(max(0,min(rigidbody_->velocity_.y_,jumpAnimVelocity)),0,jumpAnimVelocity,1,jumpAnimAmount),
		1 + sinf(modelScalingTime) * animAmount };

	nose_->transform_.localPosition_ = { 0,sinf(modelScalingTime) * animAmount,1.01f };

	if (GetInputStatusTrigger(jump))
	{
		rigidbody_->velocity_.y_ = jumpPower;
		jumpSE_->Play();
	}
	if (GetInputStatusRelease(jump) && rigidbody_->velocity_.y_ > 0)
	{
		rigidbody_->velocity_.y_ *= 0.2f;
	}
}

void Player::ViewUpdate()
{
	cameraTiltVelocity_.x_ = ADXUtility::Lerp(cameraTiltVelocity_.x_, -GetCameraControlInput().x_ * maxCameraTiltVelocity, cameraTiltForce);
	cameraTiltVelocity_.y_ = ADXUtility::Lerp(cameraTiltVelocity_.y_, GetCameraControlInput().y_ * maxCameraTiltVelocity, cameraTiltForce);

	camera_->GetGameObject()->transform_.SetWorldPosition(
		camera_->GetGameObject()->transform_.GetWorldPosition()
		+ camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 1,0,0 }) * cameraTiltVelocity_.x_);

	cameraHeight_ = max(cameraHeightLimit.x_, min(cameraHeight_ + cameraTiltVelocity_.y_ * cameraTiltSpeedY, cameraHeightLimit.y_));

	ADXVector3 cameraLocalPos = GetGameObject()->transform_.InverseTransformPoint(camera_->GetGameObject()->transform_.GetWorldPosition());
	cameraLocalPos.y_ = 0;
	cameraLocalPos = cameraLocalPos.Normalize();
	cameraLocalPos.y_ = cameraHeight_;
	camera_->GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.TransformPoint(cameraLocalPos * cameraDistance / scale_));


	camera_->GetGameObject()->transform_.SetWorldRotation(GetGameObject()->transform_.GetWorldRotation());
	cameraLocalPos = camera_->GetGameObject()->transform_.InverseTransformPoint(GetGameObject()->transform_.GetWorldPosition());
	camera_->GetGameObject()->transform_.SetWorldRotation(
		ADXQuaternion::MakeAxisAngle(camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 0,1,0 }), atan2(cameraLocalPos.x_, cameraLocalPos.z_))
		* camera_->GetGameObject()->transform_.GetWorldRotation());

	cameraLocalPos = camera_->GetGameObject()->transform_.InverseTransformPoint(GetGameObject()->transform_.GetWorldPosition());
	camera_->GetGameObject()->transform_.SetWorldRotation(
		ADXQuaternion::MakeAxisAngle(camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 1,0,0 }), atan2(-cameraLocalPos.y_, cameraLocalPos.z_))
		* camera_->GetGameObject()->transform_.GetWorldRotation());
}

void Player::LiveEntitiesInitialize()
{
	damageSE_->LoadADXAudio("sound/damage.wav");
	defeatSE_->LoadADXAudio("sound/despawn.wav");
	jumpSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	jumpSE_->LoadADXAudio("sound/jump.wav");
	jumpSE_->useDistanceFade_ = true;
	windowOpenSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	windowOpenSE_->LoadADXAudio("sound/windowOpen.wav");
	splitSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	splitSE_->LoadADXAudio("sound/split.wav");
	splitSE_->useDistanceFade_ = true;
	absorbSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	absorbSE_->LoadADXAudio("sound/split.wav");
	absorbSE_->useDistanceFade_ = true;

	rect_ = ADXModel::CreateRect();
	playerModel_ = ADXModel::LoadADXModel("model/sphere.obj");

	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();

	visual_->GetComponent<ADXModelRenderer>()->model_ = &playerModel_;
	visual_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/apEG_fur.png");

	nose_ = ADXObject::Create({ 0,0,1.01f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose_->transform_.parent_ = &visual_->transform_;
	ADXModelRenderer* tempRenderer = nose_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	tempRenderer->material_ = visual_->GetComponent<ADXModelRenderer>()->material_;
	bodyParts_.push_back(nose_);

	smile_ = ADXObject::Create();
	smile_->transform_.parent_ = &visual_->transform_;
	tempRenderer = smile_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEG_smile.png");
	tempRenderer->material_ = visual_->GetComponent<ADXModelRenderer>()->material_;
	bodyParts_.push_back(smile_);

	ADXObject* temp = ADXObject::Create();
	temp->AddComponent<ADXAudioListener>();
	temp->GetComponent<ADXAudioListener>()->radius_ = listenerRadius;
	camera_ = temp->AddComponent<ADXCamera>();
	camera_->GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.TransformPoint({ 0,0,-1 }));
	camera_->GetGameObject()->transform_.SetWorldRotation(GetGameObject()->transform_.GetWorldRotation());
	camera_->GetGameObject()->transform_.UpdateMatrix();

	tutorialWindow_ = ADXObject::Create();
	tutorialWindow_->transform_.rectTransform_ = true;
	tempRenderer = tutorialWindow_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.png");
	tempRenderer->material_ = visual_->GetComponent<ADXModelRenderer>()->material_;
	tutorialWindow_->renderLayer_ = 1;

	outOfField_ = ADXObject::Create();
	outOfField_->transform_.rectTransform_ = true;
	tempRenderer = outOfField_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/outOfField.png");
	tempRenderer->material_ = visual_->GetComponent<ADXModelRenderer>()->material_;
	outOfField_->renderLayer_ = 1;

	gameOverFilter_ = ADXObject::Create();
	gameOverFilter_->transform_.rectTransform_ = true;
	tempRenderer = gameOverFilter_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.png");
	tempRenderer->material_ = visual_->GetComponent<ADXModelRenderer>()->material_;
	gameOverFilter_->renderLayer_ = 1;

	dead_ = ADXObject::Create();
	dead_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });
	dead_->transform_.localScale_ = { 1.5f,1.5f,1.5f };
	dead_->transform_.parent_ = &GetGameObject()->transform_;
	tempRenderer = dead_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEG_dead.png");
	tempRenderer->material_ = visual_->GetComponent<ADXModelRenderer>()->material_;
	dead_->renderLayer_ = gameOverLayer;

	keyUI_ = ADXObject::Create();
	keyUI_->transform_.rectTransform_ = true;
	tempRenderer = keyUI_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");
	keyUI_->renderLayer_ = uiLayer;

	shardParticle_ = GetGameObject()->AddComponent<ADXParticleSystem>();
	shardParticle_->animation_.Initialize({
		ADXImage::LoadADXImage("texture/apEG_shard.png"), }, 0, false);
	shardParticle_->lifeTime_ = shardParticle_->animation_.GetLength();
	shardParticle_->particleModel_ = rect_;

	deadParticle_ = GetGameObject()->AddComponent<ADXParticleSystem>();
	deadParticle_->animation_.Initialize({
		ADXImage::LoadADXImage("texture/particle_defeat/000.png"), ADXImage::LoadADXImage("texture/particle_defeat/001.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/002.png"), ADXImage::LoadADXImage("texture/particle_defeat/003.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/004.png"), ADXImage::LoadADXImage("texture/particle_defeat/005.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/006.png"), ADXImage::LoadADXImage("texture/particle_defeat/007.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/008.png"), ADXImage::LoadADXImage("texture/particle_defeat/009.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/010.png"), }, 0, false);
	deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength();
	deadParticle_->particleModel_ = rect_;

	deathCountIcon_ = ADXObject::Create();
	deathCountIcon_->transform_.rectTransform_ = true;
	tempRenderer = deathCountIcon_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEG_dead.png");
	deathCountIcon_->renderLayer_ = uiLayer;
	deathCountIcon_->transform_.localPosition_ = { 0.85f,0.5f,0 };
	deathCountIcon_->transform_.localScale_ = { 0,0,0 };

	deathCountUI_ = ADXObject::Create();
	deathCountUI_->transform_.rectTransform_ = true;
	deathCountUI_->transform_.parent_ = &deathCountIcon_->transform_;
	deathCountUI_->renderLayer_ = uiLayer;
	deathCountUI_->AddComponent<ADXTextRenderer>();
	deathCountUI_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	deathCountUI_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	deathCountUI_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	deathCountUI_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::middleRight;
	deathCountUI_->transform_.localPosition_ = { -1.5f,-0.25f,0 };
	deathCountUI_->transform_.localScale_ *= 0.75f;

	killCountIcon_ = ADXObject::Create();
	killCountIcon_->transform_.rectTransform_ = true;
	tempRenderer = killCountIcon_->AddComponent<ADXModelRenderer>();
	killCountIcon_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	killCountIcon_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/Cub_E_dead.png");
	killCountIcon_->renderLayer_ = uiLayer;
	killCountIcon_->transform_.localPosition_ = { 0.85f,0.8f,0 };
	killCountIcon_->transform_.localScale_ = { 0,0,0 };

	killCountUI_ = ADXObject::Create();
	killCountUI_->transform_.rectTransform_ = true;
	killCountUI_->transform_.parent_ = &killCountIcon_->transform_;
	killCountUI_->renderLayer_ = uiLayer;
	killCountUI_->AddComponent<ADXTextRenderer>();
	killCountUI_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	killCountUI_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	killCountUI_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	killCountUI_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::middleRight;
	killCountUI_->transform_.localPosition_ = { -1.5f,-0.25f,0 };
	killCountUI_->transform_.localScale_ *= 0.75f;

	temp = ADXObject::Create();
	temp->transform_.rectTransform_ = true;
	temp->renderLayer_ = uiLayer;
	controlTextVec_ = temp->AddComponent<ADXTextRenderer>();
	controlTextVec_->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	controlTextVec_->fontAspect_ = 0.75f;
	controlTextVec_->fontExtend_ = 2;
	controlTextVec_->anchor_ = ADXTextRenderer::middleLeft;
	controlTextVec_->GetGameObject()->transform_.localPosition_ = { -0.9f,-0.45f,0 };
	controlTextVec_->GetGameObject()->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	controlTextVec_->GetGameObject()->transform_.localScale_ *= 0.05f;

	temp = ADXObject::Create();
	temp->transform_.rectTransform_ = true;
	temp->renderLayer_ = uiLayer;
	controlTextCam_ = temp->AddComponent<ADXTextRenderer>();
	controlTextCam_->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	controlTextCam_->fontAspect_ = 0.75f;
	controlTextCam_->fontExtend_ = 2;
	controlTextCam_->anchor_ = ADXTextRenderer::middleLeft;
	controlTextCam_->GetGameObject()->transform_.localPosition_ = {-0.9f,-0.6f,0};
	controlTextCam_->GetGameObject()->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	controlTextCam_->GetGameObject()->transform_.localScale_ *= 0.05f;

	temp = ADXObject::Create();
	temp->transform_.rectTransform_ = true;
	temp->renderLayer_ = uiLayer;
	controlTextJump_ = temp->AddComponent<ADXTextRenderer>();
	controlTextJump_->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	controlTextJump_->fontAspect_ = 0.75f;
	controlTextJump_->fontExtend_ = 2;
	controlTextJump_->anchor_ = ADXTextRenderer::middleLeft;
	controlTextJump_->GetGameObject()->transform_.localPosition_ = { -0.9f,-0.75f,0 };
	controlTextJump_->GetGameObject()->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	controlTextJump_->GetGameObject()->transform_.localScale_ *= 0.05f;

	temp = ADXObject::Create();
	temp->transform_.rectTransform_ = true;
	temp->renderLayer_ = uiLayer;
	controlTextAct_ = temp->AddComponent<ADXTextRenderer>();
	controlTextAct_->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	controlTextAct_->fontAspect_ = 0.75f;
	controlTextAct_->fontExtend_ = 2;
	controlTextAct_->anchor_ = ADXTextRenderer::middleLeft;
	controlTextAct_->GetGameObject()->transform_.localPosition_ = { -0.9f,-0.9f,0 };
	controlTextAct_->GetGameObject()->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	controlTextAct_->GetGameObject()->transform_.localScale_ *= 0.05f;

	targetRot_ = GetGameObject()->transform_.localRotation_;
}

void Player::LiveEntitiesUpdate()
{
	//操作説明テキストの初期値設定
	controlTextVec_->text_ = "move";
	controlTextCam_->text_ = "camera tilt";
	controlTextJump_->text_ = "jump";
	controlTextAct_->text_ = "split";

	visual_->renderLayer_ = 0;
	nose_->renderLayer_ = 0;
	smile_->renderLayer_ = 0;

	gameOverFilter_->isVisible_ =
		dead_->isVisible_ =
		keyUI_->isVisible_ = false;

	visual_->isVisible_ =
		nose_->isVisible_ = !Goal::Goaled();

	smile_->isVisible_ = Goal::Goaled();

	deadAnimationProgress_ = 0;

	scale_ = ADXUtility::ValueMapping((float)minis_.size(), 0, (float)maxMinisNum, 1, 0.25f);
	GetGameObject()->transform_.localScale_ = { scale_,scale_,scale_ };

	visual_->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	ViewUpdate();

	rigidbody_->drag_ = 0.8f;

	rigidbody_->VelocityMove();

	if (minis_.size() >= maxMinisNum)
	{
		controlTextAct_->text_ = "fix";
	}
	else if (minis_.size() != 0)
	{
		controlTextAct_->text_ = "split/fix";
	}

	if (GetInputStatus(attack))
	{
		rigidbody_->velocity_ *= 0.8f;
		rigidbody_->gravityScale_ = 0;
		if ((prevInputVec_.Length() == 0 && GetDirectionInput().Length() != 0)
			|| (prevInputVec_.Length() != 0 && GetDirectionInput().Length() == 0)
			|| GetInputStatusTrigger(jump) || GetInputStatusRelease(jump))
		{
			splitable_ = false;
		}

		if (splitable_)
		{
			if (minis_.size() >= maxMinisNum)
			{
				controlTextAct_->text_ = "fix";
			}
		}
		else
		{
			if(minis_.size() == 0)
			{
				controlTextAct_->text_ = "_";
			}
			else
			{
				controlTextAct_->text_ = "fix";
			}
		}

		if (minis_.size() == 0)
		{
			controlTextVec_->text_ = "_";
			controlTextJump_->text_ = "_";
		}
		else
		{
			controlTextVec_->text_ = "move squad";
			controlTextJump_->text_ = "jump squad";
		}
	}
	else
	{
		rigidbody_->gravityScale_ = 0.01f;
		rigidbody_->dragAxis_.y = false;
		Move(0.05f, 0.4f);
	}

	GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot_, aimSpeed);

	for (auto& itr : minis_)
	{
		if (itr->GetGameObject() != nullptr
			&& ADXMatrix4::Transform(itr->GetGameObject()->transform_.localPosition_, GetGameObject()->transform_.GetMatWorldInverse()).Length() > 30 / scale_)
		{
			itr->GetGameObject()->Destroy();
			absorbSE_->Play();
		}
	}

	nose_->transform_.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } *(float)fmax(1, 1 + pow(fmax(0, splitInterval_), 2) * 0.02f);
	nose_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });

	splitInterval_--;
	splitInterval_ = max(-20, splitInterval_);

	if (splitable_ && GetInputStatusRelease(attack) && splitInterval_ <= 0)
	{
		nose_->transform_.localScale_ = { 0.42f,0.35f,0.35f };
		if (minis_.size() < maxMinisNum)
		{
			ADXObject* miniObj = ADXObject::Create();
			PlayerMini* mini = miniObj->AddComponent<PlayerMini>();
			
			mini->GetGameObject()->transform_.localScale_ = { 0.5f,0.5f,0.5f };
			mini->GetGameObject()->transform_.SetWorldRotation(GetGameObject()->transform_.GetWorldRotation());
			mini->GetGameObject()->transform_.localPosition_ = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform_.GetMatWorld());
			
			mini->Initialize(this);
			minis_.push_back(mini);

			shardParticle_->animation_.delayFrame_ = 0;
			shardParticle_->lifeTime_ = 15;
			for (int i = 0; i < 5; i++)
			{
				shardParticle_->Emission();
				shardParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ =
					nose_->transform_.localPosition_
					+ ADXVector3{ ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1) }.Normalize() * 0.2f;
				shardParticle_->particles_.back()->velocity_ =
					ADXVector3{ ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1) }.Normalize() * ADXUtility::RandomRange(0.2f, 0.4f)
					+ ADXVector3{ 0,0,0.4f };
				shardParticle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.1f, 0.4f);
				shardParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });

			}

			splitSE_->Play();
		}
		splitInterval_ = 7;
	}

	if (GetInputStatusTrigger(attack))
	{
		splitable_ = true;
	}

	for (auto& itr : minis_)
	{
		for (auto& colItr : itr->GetGameObject()->GetComponents<ADXCollider>())
		{
			if (colItr->isTrigger_)
			{
				SetAttackObj({ colItr, this, (float)minis_.size() });
			}
		}
	}

	uint32_t prevTutorialImg = tutorialWindow_->GetComponent<ADXModelRenderer>()->texture_;

	if (prevTutorialImg != setTutorialImg_)
	{
		windowExtend_ = false;
	}
	if (windowOpening_)
	{
		windowExtend_ = true;
	}


	bool prevwindowOpening = windowOpening_;
	if (windowExtend_)
	{
		tutorialWindowExAmount_ += 0.1f;
		windowOpening_ = true;
	}
	else
	{
		tutorialWindowExAmount_ -= 0.1f;
		windowClosing_ = true;
	}
	tutorialWindowExAmount_ = max(0, min(tutorialWindowExAmount_, 1));

	if (tutorialWindowExAmount_ >= 1 || tutorialWindowExAmount_ <= 0)
	{
		windowOpening_ = false;
		windowClosing_ = false;
	}


	if (windowOpening_ && !prevwindowOpening)
	{
		windowOpenSE_->Play();
	}

	if (!windowClosing_)
	{
		tutorialWindow_->GetComponent<ADXModelRenderer>()->texture_ = setTutorialImg_;
	}

	tutorialWindow_->transform_.localScale_ = ADXUtility::Lerp({ 0,tutorialWindowSize,0 }, { tutorialWindowSize / ADXWindow::GetInstance()->GetAspect(),tutorialWindowSize,0 }, ADXUtility::EaseOut(tutorialWindowExAmount_, 4));
	tutorialWindow_->transform_.localPosition_ = { 0.65f,-0.65f + sin(clock() * 0.002f) * 0.01f,0 };


	if (isOutOfField_)
	{
		outOfField_->transform_.localScale_ = { 0.4f,0.7f,0 };
	}
	else
	{
		outOfField_->transform_.localScale_ = { 0,0,0 };
	}

	outOfField_->transform_.localPosition_ = { -0.6f,0.65f + sin(clock() * 0.003f) * 0.01f,0 };

	isOutOfField_ = true;
	windowExtend_ = false;

	deathCountUI_->GetComponent<ADXTextRenderer>()->text_ = std::to_string(deathCount_);
	deathCountIcon_->transform_.localScale_ = { deathCountIcon_->transform_.localScale_.x_,deathCountUISize,deathCountUISize };
	if (deathCount_ > 0)
	{
		deathCountIcon_->transform_.localScale_.x_ += (deathCountUISize / ADXWindow::GetInstance()->GetAspect() - deathCountIcon_->transform_.localScale_.x_) * uiExtendSpeed;
	}
	else
	{
		deathCountIcon_->transform_.localScale_.x_ -= deathCountIcon_->transform_.localScale_.x_ * uiExtendSpeed;
	}

	killCountUI_->GetComponent<ADXTextRenderer>()->text_ = std::to_string(GetKillCount());
	killCountIcon_->transform_.localScale_ = { killCountIcon_->transform_.localScale_.x_,deathCountUISize,deathCountUISize };
	if (GetKillCount() > 0)
	{
		killCountIcon_->transform_.localScale_.x_ += (deathCountUISize / ADXWindow::GetInstance()->GetAspect() - killCountIcon_->transform_.localScale_.x_) * uiExtendSpeed;
	}
	else
	{
		killCountIcon_->transform_.localScale_.x_ -= killCountIcon_->transform_.localScale_.x_ * uiExtendSpeed;
	}

	prevInputVec_ = GetDirectionInput();

	controlTextVec_->text_ = "[ARROW]" + controlTextVec_->text_;
	controlTextCam_->text_ = "[WASD ]" + controlTextCam_->text_;
	controlTextJump_->text_ = "[SPACE]" + controlTextJump_->text_;
	controlTextAct_->text_ = "[  C  ]" + controlTextAct_->text_;

	controlTextVec_->material_.ambient_ = { 1,1,1 };
	controlTextCam_->material_.ambient_ = { 1,1,1 };
	controlTextJump_->material_.ambient_ = { 1,1,1 };
	controlTextAct_->material_.ambient_ = { 1,1,1 };

	if (GetDirectionInput() != ADXVector2{ 0,0 })
	{
		controlTextVec_->material_.ambient_ = { 1,1,0 };
	}
	if (GetCameraControlInput() != ADXVector2{ 0,0 })
	{
		controlTextCam_->material_.ambient_ = { 1,1,0 };
	}
	if (GetInputStatus(jump))
	{
		controlTextJump_->material_.ambient_ = { 1,1,0 };
	}
	if (GetInputStatus(attack))
	{
		controlTextAct_->material_.ambient_ = { 1,1,0 };
	}

	for (auto& itr : shardParticle_->particles_)
	{
		itr->velocity_ *= 0.8f;
		itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.2f }) * itr->GetGameObject()->transform_.modelRotation_;
		itr->scaleRate_ = ADXUtility::EaseOut(sinf((float)itr->lifeTime_ / itr->maxLifeTime_ * ADXUtility::Pi),2);
	}

	smile_->transform_.SetWorldRotation(camera_->GetGameObject()->transform_.GetWorldRotation());
}

void Player::LiveEntitiesOnCollisionHit(ADXCollider* col, [[maybe_unused]]ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<FieldBox>() != nullptr
		&& (GetGameObject()->transform_.GetWorldPosition() - col->ClosestPoint(GetGameObject()->transform_.GetWorldPosition())).Length() < 0.1f)
	{
		isOutOfField_ = false;
	}

	if (col->GetGameObject()->GetComponent<TutorialArea>() != nullptr)
	{
		if (!windowClosing_)
		{
			windowExtend_ = true;
		}
		setTutorialImg_ = col->GetGameObject()->GetComponent<TutorialArea>()->GetTutorialImg();
	}

	if (GetInputStatus(attack) && splitInterval_ <= -20)
	{
		for (auto& itr : minis_)
		{
			if (col->GetGameObject() == itr->GetGameObject())
			{
				splitable_ = false;

				shardParticle_->animation_.delayFrame_ = 0;

				shardParticle_->lifeTime_ = 5;
				shardParticle_->Emission();
				shardParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ =
					GetGameObject()->transform_.InverseTransformPoint(itr->GetGameObject()->transform_.GetWorldPosition());
				shardParticle_->particles_.back()->scale_ = 1;
				shardParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });

				shardParticle_->lifeTime_ = 15;
				for (int i = 0; i < 4; i++)
				{
					shardParticle_->Emission();
					shardParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ =
						GetGameObject()->transform_.InverseTransformPoint(itr->GetGameObject()->transform_.GetWorldPosition());
					+ADXVector3{ ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1) }.Normalize() * 0.2f;
					shardParticle_->particles_.back()->velocity_ =
						ADXVector3{ ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1),ADXUtility::RandomRange(-1,1) }.Normalize() * ADXUtility::RandomRange(0.2f, 0.4f);
					shardParticle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.1f, 0.4f);
					shardParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				}

				itr->GetGameObject()->Destroy();
				absorbSE_->Play();
			}
		}
	}
}

void Player::DeadUpdate()
{
	for (auto& itr : minis_)
	{
		itr->GetGameObject()->Destroy();
		absorbSE_->Play();
	}

	bool prevDeadIsVisible = dead_->isVisible_;

	ViewUpdate();

	gameOverFilter_->isVisible_ =
		dead_->isVisible_ =
		keyUI_->isVisible_ = true;

	gameOverFilter_->renderLayer_ = gameOverFilterLayer;
	gameOverFilter_->GetComponent<ADXModelRenderer>()->material_.ambient_ = { 0,0,0 };
	gameOverFilter_->GetComponent<ADXModelRenderer>()->material_.alpha_ = 0.8f;

	visual_->renderLayer_ = gameOverLayer;
	nose_->renderLayer_ = gameOverLayer;

	ADXVector3 cameraRelativePos = GetGameObject()->transform_.InverseTransformPoint(
		camera_->GetGameObject()->transform_.GetWorldPosition());
	visual_->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle({ 0,1,0 },
		(float)atan2(cameraRelativePos.x_ - visual_->transform_.localPosition_.x_, cameraRelativePos.z_ - visual_->transform_.localPosition_.z_));

	nose_->transform_.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f };
	nose_->transform_.localPosition_ = { 0,sin(min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress_,0.3f,0.8f,0.0f,1.0f)
		), 1) * ADXUtility::Pi) * 5,1.01f };
	nose_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress_,0.3f,0.8f,0.0f,1.0f)
		), 1) * 50 });
	dead_->transform_.localPosition_ = { 0,0,0 };
	dead_->transform_.SetWorldRotation(camera_->GetGameObject()->transform_.GetWorldRotation());

	visual_->isVisible_ = deadAnimationProgress_ < 0.3f;
	nose_->isVisible_ = deadAnimationProgress_ < 0.8f;
	dead_->isVisible_ = !nose_->isVisible_;

	keyUI_->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI_->transform_.localScale_ = { keyUI_->transform_.localScale_.x_,0.45f,1 };

	if (restartAnimationAble_)
	{
		deadAnimationProgress_ = 1;

		if (restartAnimationProgress_ < 0.2f)
		{
			int shakeAngle = rand();
			dead_->transform_.localPosition_ = ADXVector3{ sinf((float)shakeAngle),cosf((float)shakeAngle),0 } * 0.04f;
		}
		else if (restartAnimationProgress_ < 1)
		{
			dead_->transform_.localPosition_ = { 0, powf(ADXUtility::ValueMapping(restartAnimationProgress_, 0.2f, 1.0f, 0.0f, 1.0f),3) * 20,0 };
		}
		else
		{
			SceneTransition::ChangeScene(2);
		}

		restartAnimationProgress_ = min(max(0, restartAnimationProgress_ + 0.015f), 1);
	}
	else
	{

		if (deadAnimationProgress_ >= 1)
		{
			keyUI_->transform_.localScale_.x_ += (0.45f / ADXWindow::GetInstance()->GetAspect() - keyUI_->transform_.localScale_.x_) * uiExtendSpeed;
			/*if (GetInputStatusTrigger(attack))
			{
				restartAnimationAble_ = true;
			}*/
			if (GetInputStatusTrigger(jump) || GetInputStatusTrigger(attack))
			{
				deathCount_++;
				Revive();
			}
		}
		else
		{
			keyUI_->transform_.localScale_.x_ = 0;
			if (deadAnimationProgress_ > 0.3f
				&& (GetInputStatusTrigger(jump) || GetInputStatusTrigger(attack)))
			{
				deadAnimationProgress_ = 1;
			}
		}


		if (!visual_->isVisible_ && nose_->isVisible_)
		{
			deadParticle_->animation_.delayFrame_ = 0;
			deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength();
			deadParticle_->Emission();
			deadParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ =
				GetGameObject()->transform_.InverseTransformPoint(nose_->transform_.GetWorldPosition())
				+ ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
			deadParticle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.1f;
			deadParticle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.1f, 0.3f);
			deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = gameOverLayer;
		}

		if (dead_->isVisible_ && !prevDeadIsVisible)
		{
			deadParticle_->animation_.delayFrame_ = 0;
			deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength();
			for (int i = 0; i < 9; i++)
			{
				deadParticle_->Emission();
				deadParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
				deadParticle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.3f;
				deadParticle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.9f, 2.7f);
				deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = uiLayer;
			}
			deadParticle_->animation_.delayFrame_ = 2;
			deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength() * 3;
			for (int i = 0; i < 9; i++)
			{
				deadParticle_->Emission();
				deadParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
				deadParticle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.3f;
				deadParticle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.15f, 0.45f);
				deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = gameOverLayer;
			}
		}

		for (auto& itr : deadParticle_->particles_)
		{
			itr->velocity_ *= 0.9f;
			itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform_.modelRotation_;
		}

		for (auto& itr : shardParticle_->particles_)
		{
			itr->velocity_ *= 0.9f;
			itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.3f }) * itr->GetGameObject()->transform_.modelRotation_;
		}

		deadAnimationProgress_ = min(max(0, deadAnimationProgress_ + 0.01f), 1);
	}

	rigidbody_->velocity_ = { 0,0,0 };

	deathCountUI_->GetComponent<ADXTextRenderer>()->text_ = std::to_string(deathCount_);
	deathCountIcon_->transform_.localScale_ = { deathCountIcon_->transform_.localScale_.x_,deathCountUISize,deathCountUISize };
	if (deathCount_ > 0)
	{
		deathCountIcon_->transform_.localScale_.x_ += (deathCountUISize / ADXWindow::GetInstance()->GetAspect() - deathCountIcon_->transform_.localScale_.x_) * uiExtendSpeed;
	}
	else
	{
		deathCountIcon_->transform_.localScale_.x_ -= deathCountIcon_->transform_.localScale_.x_ * uiExtendSpeed;
	}

	killCountUI_->GetComponent<ADXTextRenderer>()->text_ = std::to_string(GetKillCount());
	killCountIcon_->transform_.localScale_ = { killCountIcon_->transform_.localScale_.x_,deathCountUISize,deathCountUISize };
	if (GetKillCount() > 0)
	{
		killCountIcon_->transform_.localScale_.x_ += (deathCountUISize / ADXWindow::GetInstance()->GetAspect() - killCountIcon_->transform_.localScale_.x_) * uiExtendSpeed;
	}
	else
	{
		killCountIcon_->transform_.localScale_.x_ -= killCountIcon_->transform_.localScale_.x_ * uiExtendSpeed;
	}
}

void Player::SafetyPhase()
{
	minis_.remove_if([=](auto& itr)
		{ return itr->GetGameObject()->GetDeleteFlag(); });
}
