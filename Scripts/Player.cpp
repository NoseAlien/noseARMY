﻿#include "Player.h"
#include "SceneTransition.h"
#include "ADXUtility.h"
#include <time.h>

void Player::Initialize(ADXKeyBoardInput* setKeyboard, const keyboardConfig& setKeyBoardConfig,
	ADXGamePadInput* setGamePad, const gamePadConfig& setGamePadConfig,
	ADXCamera* setCamera)
{
	keyboard_ = setKeyboard;
	keyboardConfig_ = setKeyBoardConfig;
	gamePad_ = setGamePad;
	gamePadConfig_ = setGamePadConfig;
	camera_ = setCamera;
}

bool Player::GetInputStatus(actionsList action)
{
	switch (action)
	{
	case jump:
		return keyboard_->GetKey(keyboardConfig_.jump);
		break;
	case attack:
		return keyboard_->GetKey(keyboardConfig_.attack);
		break;
	}
	return false;
}

bool Player::GetInputStatusTrigger(actionsList action)
{
	switch (action)
	{
	case jump:
		return keyboard_->GetKeyDown(keyboardConfig_.jump);
		break;
	case attack:
		return keyboard_->GetKeyDown(keyboardConfig_.attack);
		break;
	}
	return false;
}

bool Player::GetInputStatusRelease(actionsList action)
{
	switch (action)
	{
	case jump:
		return keyboard_->GetKeyUp(keyboardConfig_.jump);
		break;
	case attack:
		return keyboard_->GetKeyUp(keyboardConfig_.attack);
		break;
	}
	return false;
}

ADXVector2 Player::GetDirectionInput()
{
	ADXVector2 ret{};

	if (keyboard_->GetKey(keyboardConfig_.up))
	{
		ret.y_ += 1;
	}
	if (keyboard_->GetKey(keyboardConfig_.down))
	{
		ret.y_ -= 1;
	}
	if (keyboard_->GetKey(keyboardConfig_.right))
	{
		ret.x_ += 1;
	}
	if (keyboard_->GetKey(keyboardConfig_.left))
	{
		ret.x_ -= 1;
	}

	if (gamePad_ != nullptr)
	{
		ret += gamePad_->GetLeftStickVec();
	}

	return ret.Normalize();
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

	if (inputVec != ADXVector2{ 0,0 })
	{
		GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody_->velocity_.x_, rigidbody_->velocity_.z_),0 });
	}

	if (GetInputStatusTrigger(jump))
	{
		rigidbody_->velocity_.y_ = jumpPower;
		jumpSE_.Play();
	}
	if (GetInputStatusRelease(jump) && rigidbody_->velocity_.y_ > 0)
	{
		rigidbody_->velocity_.y_ *= 0.2f;
	}
}

void Player::ViewUpdate()
{
	ADXVector3 cameraVec = camera_->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition();
	cameraVec.y_ = 0;
	cameraVec = cameraVec.Normalize();
	camera_->GetGameObject()->transform_.localPosition_ = GetGameObject()->transform_.localPosition_ + cameraVec * 20;
	camera_->GetGameObject()->transform_.localPosition_.y_ += 5;
	camera_->GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(-cameraVec.x_, -cameraVec.z_),0 });
	camera_->GetGameObject()->transform_.UpdateMatrix();
	camera_->GetGameObject()->transform_.localRotation_ = camera_->GetGameObject()->transform_.localRotation_ * ADXQuaternion::MakeAxisAngle({ 1,0,0 }, 0.3f);
}

void Player::LiveEntitiesInitialize()
{
	jumpSE_ = ADXAudio::LoadADXAudio("sound/jump.wav");
	damageSE_ = ADXAudio::LoadADXAudio("sound/damage.wav");
	defeatSE_ = ADXAudio::LoadADXAudio("sound/despawn.wav");
	windowOpenSE_ = ADXAudio::LoadADXAudio("sound/windowOpen.wav");

	rect_ = ADXModel::CreateRect();
	playerModel_ = ADXModel::LoadADXModel("model/sphere.obj");

	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();

	visual_->model_ = &playerModel_;
	visual_->texture_ = ADXImage::LoadADXImage("texture/apEG_fur.dds");

	nose_ = ADXObject::Create({ 0,0,1.01f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose_->transform_.parent_ = &visual_->transform_;
	nose_->transform_.UpdateMatrix();
	nose_->model_ = &rect_;
	nose_->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.dds");
	nose_->material_ = GetGameObject()->material_;
	bodyParts_.push_back(nose_);

	tutorialWindow_ = ADXObject::Create();
	tutorialWindow_->transform_.rectTransform_ = true;
	tutorialWindow_->transform_.UpdateMatrix();
	tutorialWindow_->model_ = &rect_;
	tutorialWindow_->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.dds");
	tutorialWindow_->material_ = GetGameObject()->material_;
	tutorialWindow_->renderLayer_ = 1;

	outOfField_ = ADXObject::Create();
	outOfField_->transform_.rectTransform_ = true;
	outOfField_->transform_.UpdateMatrix();
	outOfField_->model_ = &rect_;
	outOfField_->texture_ = ADXImage::LoadADXImage("texture/outOfField.dds");
	outOfField_->material_ = GetGameObject()->material_;
	outOfField_->renderLayer_ = 1;

	gameOverFilter_ = ADXObject::Create();
	gameOverFilter_->transform_.rectTransform_ = true;
	gameOverFilter_->transform_.UpdateMatrix();
	gameOverFilter_->model_ = &rect_;
	gameOverFilter_->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.dds");
	gameOverFilter_->material_ = GetGameObject()->material_;
	gameOverFilter_->renderLayer_ = 1;

	dead_ = ADXObject::Create();
	dead_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });
	dead_->transform_.localScale_ = { 1.5f,1.5f,1.5f };
	dead_->transform_.parent_ = &GetGameObject()->transform_;
	dead_->transform_.UpdateMatrix();
	dead_->model_ = &rect_;
	dead_->texture_ = ADXImage::LoadADXImage("texture/apEG_dead.dds");
	dead_->material_ = GetGameObject()->material_;
	dead_->renderLayer_ = 4;

	keyUI_ = ADXObject::Create();
	keyUI_->transform_.rectTransform_ = true;
	keyUI_->model_ = &rect_;
	keyUI_->renderLayer_ = 5;
	keyUI_->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.dds");

	deadParticle_ = GetGameObject()->AddComponent<ADXParticleSystem>();
	deadParticle_->animation_.Initialize({
		ADXImage::LoadADXImage("texture/particle_defeat/000.dds"), ADXImage::LoadADXImage("texture/particle_defeat/001.dds"),
		ADXImage::LoadADXImage("texture/particle_defeat/002.dds"), ADXImage::LoadADXImage("texture/particle_defeat/003.dds"),
		ADXImage::LoadADXImage("texture/particle_defeat/004.dds"), ADXImage::LoadADXImage("texture/particle_defeat/005.dds"),
		ADXImage::LoadADXImage("texture/particle_defeat/006.dds"), ADXImage::LoadADXImage("texture/particle_defeat/007.dds"),
		ADXImage::LoadADXImage("texture/particle_defeat/008.dds"), ADXImage::LoadADXImage("texture/particle_defeat/009.dds"),
		ADXImage::LoadADXImage("texture/particle_defeat/010.dds"), }, 0, false);
	deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength();
	deadParticle_->particleModel_ = rect_;
}

void Player::LiveEntitiesUpdate()
{
	visual_->renderLayer_ = 0;
	nose_->renderLayer_ = 0;

	gameOverFilter_->isVisible_ =
		dead_->isVisible_ =
		keyUI_->isVisible_ = false;

	visual_->isVisible_ =
		nose_->isVisible_ = true;

	deadAnimationProgress_ = 0;

	float scale = ADXUtility::ValueMapping((float)minis_.size(), 0, (float)maxMinisNum, 1, 0.25f);
	GetGameObject()->transform_.localScale_ = { scale,scale,scale };

	float modelScalingTime = (float)clock() * 0.002f;
	if (!GetInputStatus(attack) && GetDirectionInput() != ADXVector2{ 0,0 })
	{
		modelScalingTime = (float)clock() * 0.015f;
	}

	visual_->transform_.localScale_ = { 1 + sinf(modelScalingTime) * 0.03f,1 + cosf(modelScalingTime) * 0.03f,1 + sinf(modelScalingTime) * 0.03f };

	ViewUpdate();

	rigidbody_->drag_ = 0.8f;

	rigidbody_->VelocityMove();

	if (GetInputStatus(attack))
	{
		rigidbody_->velocity_ *= 0.8f;
		rigidbody_->gravityScale_ = 0;
	}
	else
	{
		rigidbody_->gravityScale_ = 0.01f;
		rigidbody_->dragAxis_.y = false;
		Move(0.05f, 0.4f);
	}

	for (auto& itr : minis_)
	{
		if (itr->GetGameObject() != nullptr
			&& ADXMatrix4::Transform(itr->GetGameObject()->transform_.localPosition_, GetGameObject()->transform_.GetMatWorldInverse()).Length() > 30 / scale)
		{
			itr->GetGameObject()->Destroy();
		}
	}

	nose_->transform_.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } *(float)fmax(1, 1 + pow(fmax(0, splitInterval_), 2) * 0.02f);
	nose_->transform_.localPosition_ = { 0,sinf(modelScalingTime) * 0.03f,1.01f };

	splitInterval_--;
	splitInterval_ = max(-20, splitInterval_);

	if (splitable_ && GetInputStatusRelease(attack) && splitInterval_ <= 0)
	{
		nose_->transform_.localScale_ = { 0.42f,0.35f,0.35f };
		nose_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });
		if (minis_.size() < maxMinisNum)
		{
			
			ADXObject* miniObj = nullptr;
			miniObj = ADXObject::Create();
			PlayerMini* mini = miniObj->AddComponent<PlayerMini>();
			
			mini->GetGameObject()->transform_.localScale_ = { 0.5f,0.5f,0.5f };
			mini->GetGameObject()->transform_.SetWorldRotation(GetGameObject()->transform_.GetWorldRotation());
			mini->GetGameObject()->transform_.localPosition_ = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform_.GetMatWorld());
			
			mini->Initialize(this);
			minis_.push_back(mini);
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

	uint32_t prevTutorialImg = tutorialWindow_->texture_;

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
		windowOpenSE_.Play();
	}

	if (!windowClosing_)
	{
		tutorialWindow_->texture_ = setTutorialImg_;
	}

	tutorialWindow_->transform_.localScale_ = ADXUtility::Lerp({ 0,0.3f,0 }, { 0.3f / ADXWindow::GetAspect(),0.3f,0 }, ADXUtility::EaseOut(tutorialWindowExAmount_, 4));
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
}

void Player::LiveEntitiesOnCollisionHit(ADXCollider* col, [[maybe_unused]]ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<FieldBox>() != nullptr
		&& (GetGameObject()->transform_.GetWorldPosition() - col->ClosestPoint(GetGameObject()->transform_.GetWorldPosition())).Length() < 0.1)
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

	if (splitInterval_ <= -20)
	{
		for (auto& itr : minis_)
		{
			if (col->GetGameObject() == itr->GetGameObject())
			{
				splitable_ = false;
				itr->GetGameObject()->Destroy();
			}
		}
	}
}

void Player::DeadUpdate()
{
	for (auto& itr : minis_)
	{
		itr->GetGameObject()->Destroy();
	}

	bool prevDeadIsVisible = dead_->isVisible_;

	ViewUpdate();

	gameOverFilter_->isVisible_ =
		dead_->isVisible_ =
		keyUI_->isVisible_ = true;

	gameOverFilter_->renderLayer_ = 3;
	gameOverFilter_->material_.ambient_ = { 0,0,0 };
	gameOverFilter_->material_.alpha_ = 0.8f;

	visual_->renderLayer_ = 4;
	nose_->renderLayer_ = 4;

	GetGameObject()->transform_.SetWorldRotation(camera_->GetGameObject()->transform_.GetWorldRotation());
	GetGameObject()->transform_.UpdateMatrix();
	GetGameObject()->transform_.SetWorldRotation(
		ADXQuaternion::MakeAxisAngle(GetGameObject()->transform_.TransformPointOnlyRotation({ 0,1,0 }), ADXUtility::Pi)
		* GetGameObject()->transform_.GetWorldRotation());

	nose_->transform_.localPosition_ = { 0,sin(min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress_,0.3f,0.8f,0.0f,1.0f)
		), 1) * ADXUtility::Pi) * 5,1.01f };
	nose_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress_,0.3f,0.8f,0.0f,1.0f)
		), 1) * 50 });
	dead_->transform_.localPosition_ = { 0,0,0 };


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
			keyUI_->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI_->transform_.localScale_.x_) * 0.3f;
			if (GetInputStatusTrigger(jump))
			{
				restartAnimationAble_ = true;
			}
		}
		else
		{
			keyUI_->transform_.localScale_.x_ = 0;
			if (deadAnimationProgress_ > 0.3f
				&& GetInputStatusTrigger(jump))
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
				nose_->transform_.localPosition_ + ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
			deadParticle_->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.1f;
			float particleScale = 0.3f + (float)(rand() % 3) * 0.2f;
			deadParticle_->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } *0.3f;
			deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = 4;
		}

		if (dead_->isVisible_ && !prevDeadIsVisible)
		{
			deadParticle_->animation_.delayFrame_ = 0;
			deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength();
			for (int i = 0; i < 9; i++)
			{
				deadParticle_->Emission();
				deadParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				deadParticle_->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.3f;
				float particleScale = 0.3f + (float)(rand() % 3) * 0.2f;
				deadParticle_->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } *3;
				deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = 5;
			}
		}

		for (auto& itr : deadParticle_->particles_)
		{
			itr->moveVec_ *= 0.9f;
			itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform_.modelRotation_;
		}

		deadAnimationProgress_ = min(max(0, deadAnimationProgress_ + 0.01f), 1);
	}

	rigidbody_->velocity_ = { 0,0,0 };
}

void Player::SafetyPhase()
{
	minis_.remove_if([=](auto& itr)
		{ return itr->GetGameObject()->GetDeleteFlag(); });
}
