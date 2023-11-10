#include "Player.h"
#include "SceneTransition.h"
#include "ADXUtility.h"
#include "FieldBox.h"
#include <time.h>

void Player::Initialize(ADXKeyBoardInput* setKeyboard, const std::vector<BYTE>& setConfig, ADXCamera* setCamera)
{
	keyboard = setKeyboard;
	config = setConfig;
	camera = setCamera;
}

bool Player::GetInputStatus(int keyIndex)
{
	return keyboard->KeyPress(config[keyIndex]);
}

bool Player::GetInputStatusTrigger(int keyIndex)
{
	return keyboard->KeyTrigger(config[keyIndex]);
}

bool Player::GetInputStatusRelease(int keyIndex)
{
	return keyboard->KeyRelease(config[keyIndex]);
}

void Player::Move(float walkSpeed, float jumpPower)
{
	ADXVector3 cameraRight = camera->GetGameObject()->transform_.TransformPointOnlyRotation({ 1,0,0 });
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = camera->GetGameObject()->transform_.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y_ = 0;
	cameraForward = cameraForward.Normalize();

	if (keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3]))
	{
		if (keyboard->KeyPress(config[0]))
		{
			rigidbody->velocity_ += cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[1]))
		{
			rigidbody->velocity_ -= cameraForward * walkSpeed;
		}
		if (keyboard->KeyPress(config[2]))
		{
			rigidbody->velocity_ += cameraRight * walkSpeed;
		}
		if (keyboard->KeyPress(config[3]))
		{
			rigidbody->velocity_ -= cameraRight * walkSpeed;
		}
		GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody->velocity_.x_, rigidbody->velocity_.z_),0 });
	}

	if (keyboard->KeyTrigger(config[4]))
	{
		rigidbody->velocity_.y_ = jumpPower;
		jumpSE.Play();
	}
	if (keyboard->KeyRelease(config[4]) && rigidbody->velocity_.y_ > 0)
	{
		rigidbody->velocity_.y_ *= 0.2f;
	}
}

void Player::ViewUpdate()
{
	ADXVector3 cameraVec = camera->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition();
	cameraVec.y_ = 0;
	cameraVec = cameraVec.Normalize();
	camera->GetGameObject()->transform_.localPosition_ = GetGameObject()->transform_.localPosition_ + cameraVec * 20;
	camera->GetGameObject()->transform_.localPosition_.y_ += 5;
	camera->GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(-cameraVec.x_, -cameraVec.z_),0 });
	camera->GetGameObject()->transform_.UpdateMatrix();
	camera->GetGameObject()->transform_.localRotation_ = camera->GetGameObject()->transform_.localRotation_ * ADXQuaternion::MakeAxisAngle({ 1,0,0 }, 0.3f);
}

void Player::LiveEntitiesInitialize()
{
	jumpSE = ADXAudio::LoadADXAudio("sound/jump.wav");
	damageSE = ADXAudio::LoadADXAudio("sound/damage.wav");
	defeatSE = ADXAudio::LoadADXAudio("sound/despawn.wav");
	windowOpenSE = ADXAudio::LoadADXAudio("sound/windowOpen.wav");

	rect = ADXModel::CreateRect();
	playerModel = ADXModel::LoadADXModel("model/sphere.obj");

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();

	visual->model_ = &playerModel;
	visual->texture_ = ADXImage::LoadADXImage("texture/apEG_fur.png");

	nose = ADXObject::Create({ 0,0,1.01f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose->transform_.parent_ = &visual->transform_;
	nose->transform_.UpdateMatrix();
	nose->model_ = &rect;
	nose->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	nose->material_ = GetGameObject()->material_;
	bodyParts.push_back(nose);

	tutorialWindow = ADXObject::Create();
	tutorialWindow->transform_.rectTransform_ = true;
	tutorialWindow->transform_.UpdateMatrix();
	tutorialWindow->model_ = &rect;
	tutorialWindow->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.png");
	tutorialWindow->material_ = GetGameObject()->material_;
	tutorialWindow->renderLayer_ = 1;

	outOfField = ADXObject::Create();
	outOfField->transform_.rectTransform_ = true;
	outOfField->transform_.UpdateMatrix();
	outOfField->model_ = &rect;
	outOfField->texture_ = ADXImage::LoadADXImage("texture/outOfField.png");
	outOfField->material_ = GetGameObject()->material_;
	outOfField->renderLayer_ = 1;

	gameOverFilter = ADXObject::Create();
	gameOverFilter->transform_.rectTransform_ = true;
	gameOverFilter->transform_.UpdateMatrix();
	gameOverFilter->model_ = &rect;
	gameOverFilter->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.png");
	gameOverFilter->material_ = GetGameObject()->material_;
	gameOverFilter->renderLayer_ = 1;

	dead = ADXObject::Create();
	dead->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });
	dead->transform_.localScale_ = { 1.5f,1.5f,1.5f };
	dead->transform_.parent_ = &GetGameObject()->transform_;
	dead->transform_.UpdateMatrix();
	dead->model_ = &rect;
	dead->texture_ = ADXImage::LoadADXImage("texture/apEG_dead.png");
	dead->material_ = GetGameObject()->material_;
	dead->renderLayer_ = 4;

	keyUI = ADXObject::Create();
	keyUI->transform_.rectTransform_ = true;
	keyUI->model_ = &rect;
	keyUI->renderLayer_ = 5;
	keyUI->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");

	deadParticle = GetGameObject()->AddComponent<ADXParticleSystem>();
	deadParticle->animation_.Initialize({
		ADXImage::LoadADXImage("texture/particle_defeat/000.png"), ADXImage::LoadADXImage("texture/particle_defeat/001.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/002.png"), ADXImage::LoadADXImage("texture/particle_defeat/003.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/004.png"), ADXImage::LoadADXImage("texture/particle_defeat/005.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/006.png"), ADXImage::LoadADXImage("texture/particle_defeat/007.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/008.png"), ADXImage::LoadADXImage("texture/particle_defeat/009.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/010.png"), }, 0, false);
	deadParticle->lifeTime_ = deadParticle->animation_.GetLength();
	deadParticle->particleModel_ = rect;
}

void Player::LiveEntitiesUpdate()
{
	visual->renderLayer_ = 0;
	nose->renderLayer_ = 0;

	gameOverFilter->isVisible_ =
		dead->isVisible_ =
		keyUI->isVisible_ = false;

	visual->isVisible_ =
		nose->isVisible_ = true;

	deadAnimationProgress = 0;

	float scale = ADXUtility::ValueMapping((float)minis.size(), 0, (float)maxMinisNum, 1, 0.25f);
	GetGameObject()->transform_.localScale_ = { scale,scale,scale };

	float modelScalingTime = (float)clock() * 0.002f;
	if (!keyboard->KeyPress(config[5]) && (keyboard->KeyPress(config[0]) || keyboard->KeyPress(config[1]) || keyboard->KeyPress(config[2]) || keyboard->KeyPress(config[3])))
	{
		modelScalingTime = (float)clock() * 0.015f;
	}

	visual->transform_.localScale_ = { 1 + sinf(modelScalingTime) * 0.03f,1 + cosf(modelScalingTime) * 0.03f,1 + sinf(modelScalingTime) * 0.03f };

	ViewUpdate();

	rigidbody->drag_ = 0.8f;

	rigidbody->VelocityMove();

	if (keyboard->KeyPress(config[5]))
	{
		rigidbody->velocity_ *= 0.8f;
		rigidbody->gravityScale_ = 0;
	}
	else
	{
		rigidbody->gravityScale_ = 0.01f;
		rigidbody->dragAxis_.y = false;
		Move(0.05f, 0.4f);
	}

	for (auto& itr : minis)
	{
		if (itr->GetGameObject() != nullptr
			&& ADXMatrix4::Transform(itr->GetGameObject()->transform_.localPosition_, GetGameObject()->transform_.GetMatWorldInverse()).Length() > 30 / scale)
		{
			itr->GetGameObject()->Destroy();
		}
	}

	nose->transform_.localScale_ = ADXVector3{ 0.42f,0.35f,0.35f } *(float)fmax(1, 1 + pow(fmax(0, splitInterval), 2) * 0.02f);
	nose->transform_.localPosition_ = { 0,sinf(modelScalingTime) * 0.03f,1.01f };

	splitInterval--;
	splitInterval = max(-20, splitInterval);

	if (splitable && keyboard->KeyRelease(config[5]) && splitInterval <= 0)
	{
		nose->transform_.localScale_ = { 0.42f,0.35f,0.35f };
		nose->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });
		if (minis.size() < maxMinisNum)
		{
			
			ADXObject* miniObj = nullptr;
			miniObj = ADXObject::Create();
			PlayerMini* mini = miniObj->AddComponent<PlayerMini>();
			
			mini->GetGameObject()->transform_.localScale_ = { 0.5f,0.5f,0.5f };
			mini->GetGameObject()->transform_.SetWorldRotation(GetGameObject()->transform_.GetWorldRotation());
			mini->GetGameObject()->transform_.localPosition_ = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform_.GetMatWorld());
			
			mini->Initialize(this);
			minis.push_back(mini);
		}
		splitInterval = 7;
	}

	if (!keyboard->KeyPress(config[5]))
	{
		splitable = true;
	}

	for (auto& itr : minis)
	{
		for (auto& colItr : itr->GetGameObject()->GetComponents<ADXCollider>())
		{
			if (colItr->isTrigger_)
			{
				SetAttackObj({ colItr, this, (float)minis.size() });
			}
		}
	}

	uint32_t prevTutorialImg = tutorialWindow->texture_;

	if (prevTutorialImg != setTutorialImg)
	{
		windowExtend = false;
	}
	if (windowOpening)
	{
		windowExtend = true;
	}


	bool prevwindowOpening = windowOpening;
	if (windowExtend)
	{
		tutorialWindowExAmount += 0.1f;
		windowOpening = true;
	}
	else
	{
		tutorialWindowExAmount -= 0.1f;
		windowClosing = true;
	}
	tutorialWindowExAmount = max(0, min(tutorialWindowExAmount, 1));

	if (tutorialWindowExAmount >= 1 || tutorialWindowExAmount <= 0)
	{
		windowOpening = false;
		windowClosing = false;
	}


	if (windowOpening && !prevwindowOpening)
	{
		windowOpenSE.Play();
	}

	if (!windowClosing)
	{
		tutorialWindow->texture_ = setTutorialImg;
	}

	tutorialWindow->transform_.localScale_ = ADXUtility::Lerp({ 0,0.3f,0 }, { 0.3f / ADXWindow::GetAspect(),0.3f,0 }, ADXUtility::EaseOut(tutorialWindowExAmount, 4));
	tutorialWindow->transform_.localPosition_ = { 0.65f,-0.65f + sin(clock() * 0.002f) * 0.01f,0 };


	if (isOutOfField)
	{
		outOfField->transform_.localScale_ = { 0.4f,0.7f,0 };
	}
	else
	{
		outOfField->transform_.localScale_ = { 0,0,0 };
	}

	outOfField->transform_.localPosition_ = { -0.6f,0.65f + sin(clock() * 0.003f) * 0.01f,0 };

	isOutOfField = true;
	windowExtend = false;
}

void Player::LiveEntitiesOnCollisionHit(ADXCollider* col, [[maybe_unused]]ADXCollider* myCol)
{
	if (col->GetGameObject()->GetComponent<FieldBox>() != nullptr
		&& (GetGameObject()->transform_.GetWorldPosition() - col->ClosestPoint(GetGameObject()->transform_.GetWorldPosition())).Length() < 0.1)
	{
		isOutOfField = false;
	}

	if (col->GetGameObject()->GetComponent<TutorialArea>() != nullptr)
	{
		if (!windowClosing)
		{
			windowExtend = true;
		}
		setTutorialImg = col->GetGameObject()->GetComponent<TutorialArea>()->GetTutorialImg();
	}

	if (splitInterval <= -20)
	{
		for (auto& itr : minis)
		{
			if (col->GetGameObject() == itr->GetGameObject())
			{
				splitable = false;
				itr->GetGameObject()->Destroy();
			}
		}
	}
}

void Player::DeadUpdate()
{
	bool prevDeadIsVisible = dead->isVisible_;

	ViewUpdate();

	gameOverFilter->isVisible_ =
		dead->isVisible_ =
		keyUI->isVisible_ = true;

	gameOverFilter->renderLayer_ = 3;
	gameOverFilter->material_.ambient_ = { 0,0,0 };
	gameOverFilter->material_.alpha_ = 0.8f;

	visual->renderLayer_ = 4;
	nose->renderLayer_ = 4;

	GetGameObject()->transform_.SetWorldRotation(camera->GetGameObject()->transform_.GetWorldRotation());
	GetGameObject()->transform_.UpdateMatrix();
	GetGameObject()->transform_.SetWorldRotation(
		ADXQuaternion::MakeAxisAngle(GetGameObject()->transform_.TransformPointOnlyRotation({ 0,1,0 }), ADXUtility::Pi)
		* GetGameObject()->transform_.GetWorldRotation());

	nose->transform_.localPosition_ = { 0,sin(min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress,0.3f,0.8f,0.0f,1.0f)
		), 1) * ADXUtility::Pi) * 5,1.01f };
	nose->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,min(max(0,
		ADXUtility::ValueMapping(deadAnimationProgress,0.3f,0.8f,0.0f,1.0f)
		), 1) * 50 });
	dead->transform_.localPosition_ = { 0,0,0 };


	visual->isVisible_ = deadAnimationProgress < 0.3f;
	nose->isVisible_ = deadAnimationProgress < 0.8f;
	dead->isVisible_ = !nose->isVisible_;

	keyUI->transform_.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI->transform_.localScale_ = { keyUI->transform_.localScale_.x_,0.45f,1 };

	if (restartAnimationAble)
	{
		deadAnimationProgress = 1;

		if (restartAnimationProgress < 0.3f)
		{
			int shakeAngle = rand();
			dead->transform_.localPosition_ = ADXVector3{ sinf((float)shakeAngle),cosf((float)shakeAngle),0 } * 0.1f;
		}
		else if (restartAnimationProgress < 1)
		{
			dead->transform_.localPosition_ = { 0, powf(ADXUtility::ValueMapping(restartAnimationProgress, 0.3f, 1.0f, 0.0f, 1.0f),3) * 20,0 };
		}
		else
		{
			SceneTransition::ChangeScene(2);
		}

		restartAnimationProgress = min(max(0, restartAnimationProgress + 0.01f), 1);
	}
	else
	{

		if (deadAnimationProgress >= 1)
		{
			keyUI->transform_.localScale_.x_ += (0.45f / ADXWindow::GetAspect() - keyUI->transform_.localScale_.x_) * 0.3f;
			if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
			{
				restartAnimationAble = true;
			}
		}
		else
		{
			keyUI->transform_.localScale_.x_ = 0;
			if (deadAnimationProgress > 0.3f
				&& ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
			{
				deadAnimationProgress = 1;
			}
		}


		if (!visual->isVisible_ && nose->isVisible_)
		{
			deadParticle->animation_.delayFrame_ = 0;
			deadParticle->lifeTime_ = deadParticle->animation_.GetLength();
			deadParticle->Emission();
			deadParticle->particles_.back()->GetGameObject()->transform_.localPosition_ =
				nose->transform_.localPosition_ + ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
			deadParticle->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.1f;
			float particleScale = 0.3f + (float)(rand() % 3) * 0.2f;
			deadParticle->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } *0.3f;
			deadParticle->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			deadParticle->particles_.back()->GetGameObject()->renderLayer_ = 4;
		}

		if (dead->isVisible_ && !prevDeadIsVisible)
		{
			deadParticle->animation_.delayFrame_ = 0;
			deadParticle->lifeTime_ = deadParticle->animation_.GetLength();
			for (int i = 0; i < 9; i++)
			{
				deadParticle->Emission();
				deadParticle->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				deadParticle->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.3f;
				float particleScale = 0.3f + (float)(rand() % 3) * 0.2f;
				deadParticle->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } *3;
				deadParticle->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				deadParticle->particles_.back()->GetGameObject()->renderLayer_ = 5;
			}
		}

		for (auto& itr : deadParticle->particles_)
		{
			itr->moveVec_ *= 0.9f;
			itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform_.modelRotation_;
		}

		deadAnimationProgress = min(max(0, deadAnimationProgress + 0.01f), 1);
	}

	rigidbody->velocity_ = { 0,0,0 };
}

void Player::SafetyPhase()
{
	minis.remove_if([=](auto& itr)
		{ return itr->GetGameObject()->GetDeleteFlag(); });
}
