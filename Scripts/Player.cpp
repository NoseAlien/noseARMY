#include "Player.h"
#include "SceneTransition.h"
#include "ADXUtility.h"
#include "ADXKeyConfig.h"
#include "ADXAudioListener.h"
#include <time.h>

const float uiExtendSpeed = 0.3f;
const float tutorialWindowSize = 0.3f;
const float deathCountUISize = 0.1f;
const float maxCameraTiltVelocity = 0.7f;
const float cameraTiltForce = 0.3f;
const float cameraDistance = 20;
const float listenerRadius = 40;
const float idolAnimAmount = 0.05f;
const float walkAnimAmount = 0.1f;
const float jumpAnimVelocity = 0.5f;
const float jumpAnimAmount = 1.5f;
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
		GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody_->velocity_.x_, rigidbody_->velocity_.z_),0 });

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
	cameraTiltVelocity_ = ADXUtility::Lerp(cameraTiltVelocity_, -GetCameraControlInput().x_ * maxCameraTiltVelocity, cameraTiltForce);
	camera_->GetGameObject()->transform_.SetWorldPosition(
		camera_->GetGameObject()->transform_.GetWorldPosition()
		+ camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 1,0,0 }) * cameraTiltVelocity_);

	ADXVector3 cameraLocalPos = GetGameObject()->transform_.InverseTransformPoint(camera_->GetGameObject()->transform_.GetWorldPosition()).Normalize();
	cameraLocalPos.y_ = 0.3f;
	camera_->GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.TransformPoint(cameraLocalPos));

	camera_->GetGameObject()->transform_.UpdateMatrix();

	ADXVector3 cameraVec = camera_->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition();
	cameraVec.y_ = 0;
	cameraVec = cameraVec.Normalize();
	camera_->GetGameObject()->transform_.SetWorldRotation(ADXQuaternion::EulerToQuaternion({ 0,atan2(-cameraVec.x_, -cameraVec.z_),0 }));
	camera_->GetGameObject()->transform_.UpdateMatrix();
	cameraLocalPos = camera_->GetGameObject()->transform_.InverseTransformPoint(GetGameObject()->transform_.GetWorldPosition());
	camera_->GetGameObject()->transform_.SetWorldRotation(
		ADXQuaternion::MakeAxisAngle(camera_->GetGameObject()->transform_.TransformPointOnlyRotation({ 1,0,0 }),atan2(-cameraLocalPos.y_, cameraLocalPos.z_))
		* camera_->GetGameObject()->transform_.GetWorldRotation());

	cameraVec = (camera_->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition()).Normalize();
	camera_->GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.localPosition_ + cameraVec * cameraDistance);
	camera_->GetGameObject()->transform_.UpdateMatrix();
}

void Player::LiveEntitiesInitialize()
{
	damageSE_->LoadADXAudio("sound/damage.wav");
	defeatSE_->LoadADXAudio("sound/despawn.wav");
	jumpSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	jumpSE_->LoadADXAudio("sound/jump.wav");
	windowOpenSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	windowOpenSE_->LoadADXAudio("sound/windowOpen.wav");

	rect_ = ADXModel::CreateRect();
	playerModel_ = ADXModel::LoadADXModel("model/sphere.obj");

	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();

	visual_->model_ = &playerModel_;
	visual_->texture_ = ADXImage::LoadADXImage("texture/apEG_fur.png");

	nose_ = ADXObject::Create({ 0,0,1.01f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose_->transform_.parent_ = &visual_->transform_;
	nose_->transform_.UpdateMatrix();
	nose_->model_ = &rect_;
	nose_->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	nose_->material_ = GetGameObject()->material_;
	bodyParts_.push_back(nose_);

	ADXObject* temp = ADXObject::Create();
	temp->AddComponent<ADXAudioListener>();
	temp->GetComponent<ADXAudioListener>()->radius_ = listenerRadius;
	camera_ = temp->AddComponent<ADXCamera>();
	camera_->GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.TransformPoint({ 0,0,-1 }));
	camera_->GetGameObject()->transform_.SetWorldRotation(GetGameObject()->transform_.GetWorldRotation());
	camera_->GetGameObject()->transform_.UpdateMatrix();

	tutorialWindow_ = ADXObject::Create();
	tutorialWindow_->transform_.rectTransform_ = true;
	tutorialWindow_->transform_.UpdateMatrix();
	tutorialWindow_->model_ = &rect_;
	tutorialWindow_->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.png");
	tutorialWindow_->material_ = GetGameObject()->material_;
	tutorialWindow_->renderLayer_ = 1;

	outOfField_ = ADXObject::Create();
	outOfField_->transform_.rectTransform_ = true;
	outOfField_->transform_.UpdateMatrix();
	outOfField_->model_ = &rect_;
	outOfField_->texture_ = ADXImage::LoadADXImage("texture/outOfField.png");
	outOfField_->material_ = GetGameObject()->material_;
	outOfField_->renderLayer_ = 1;

	gameOverFilter_ = ADXObject::Create();
	gameOverFilter_->transform_.rectTransform_ = true;
	gameOverFilter_->transform_.UpdateMatrix();
	gameOverFilter_->model_ = &rect_;
	gameOverFilter_->texture_ = ADXImage::LoadADXImage("texture/WhiteDot.png");
	gameOverFilter_->material_ = GetGameObject()->material_;
	gameOverFilter_->renderLayer_ = 1;

	dead_ = ADXObject::Create();
	dead_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 });
	dead_->transform_.localScale_ = { 1.5f,1.5f,1.5f };
	dead_->transform_.parent_ = &GetGameObject()->transform_;
	dead_->transform_.UpdateMatrix();
	dead_->model_ = &rect_;
	dead_->texture_ = ADXImage::LoadADXImage("texture/apEG_dead.png");
	dead_->material_ = GetGameObject()->material_;
	dead_->renderLayer_ = gameOverLayer;

	keyUI_ = ADXObject::Create();
	keyUI_->transform_.rectTransform_ = true;
	keyUI_->model_ = &rect_;
	keyUI_->renderLayer_ = uiLayer;
	keyUI_->texture_ = ADXImage::LoadADXImage("texture/PRESS_SPACE.png");

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
	deathCountIcon_->model_ = &rect_;
	deathCountIcon_->renderLayer_ = uiLayer;
	deathCountIcon_->texture_ = ADXImage::LoadADXImage("texture/apEG_dead.png");
	deathCountIcon_->transform_.localPosition_ = { 0.85f,0.5f,0 };
	deathCountIcon_->transform_.localScale_ = { 0,0,0 };

	deathCountUI_ = ADXObject::Create();
	deathCountUI_->transform_.rectTransform_ = true;
	deathCountUI_->transform_.parent_ = &deathCountIcon_->transform_;
	deathCountUI_->model_ = &rect_;
	deathCountUI_->renderLayer_ = uiLayer;
	deathCountUI_->useDefaultDraw_ = false;
	deathCountUI_->AddComponent<ADXTextRenderer>();
	deathCountUI_->GetComponent<ADXTextRenderer>()->font_ = ADXTextRenderer::GetFont("texture/alphaNumber");
	deathCountUI_->GetComponent<ADXTextRenderer>()->fontAspect_ = 0.75f;
	deathCountUI_->GetComponent<ADXTextRenderer>()->fontExtend_ = 2;
	deathCountUI_->GetComponent<ADXTextRenderer>()->anchor_ = ADXTextRenderer::middleRight;
	deathCountUI_->transform_.localPosition_ = { -1.5f,-0.25f,0 };
	deathCountUI_->transform_.localScale_ *= 0.75f;

	killCountIcon_ = ADXObject::Create();
	killCountIcon_->transform_.rectTransform_ = true;
	killCountIcon_->model_ = &rect_;
	killCountIcon_->renderLayer_ = uiLayer;
	killCountIcon_->texture_ = ADXImage::LoadADXImage("texture/Cub_E_dead.png");
	killCountIcon_->transform_.localPosition_ = { 0.85f,0.8f,0 };
	killCountIcon_->transform_.localScale_ = { 0,0,0 };

	killCountUI_ = ADXObject::Create();
	killCountUI_->transform_.rectTransform_ = true;
	killCountUI_->transform_.parent_ = &killCountIcon_->transform_;
	killCountUI_->model_ = &rect_;
	killCountUI_->renderLayer_ = uiLayer;
	killCountUI_->useDefaultDraw_ = false;
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
	controlTextVec_->GetGameObject()->transform_.localPosition_ = {-0.9f,-0.6f,0};
	controlTextVec_->GetGameObject()->transform_.localScale_.x_ /= ADXWindow::GetInstance()->GetAspect();
	controlTextVec_->GetGameObject()->transform_.localScale_ *= 0.05f;

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
}

void Player::LiveEntitiesUpdate()
{
	//操作説明テキストの初期値設定
	controlTextVec_->text_ = "move";
	controlTextJump_->text_ = "jump";
	controlTextAct_->text_ = "split";

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

	for (auto& itr : minis_)
	{
		if (itr->GetGameObject() != nullptr
			&& ADXMatrix4::Transform(itr->GetGameObject()->transform_.localPosition_, GetGameObject()->transform_.GetMatWorldInverse()).Length() > 30 / scale)
		{
			itr->GetGameObject()->Destroy();
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
		windowOpenSE_->Play();
	}

	if (!windowClosing_)
	{
		tutorialWindow_->texture_ = setTutorialImg_;
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
	controlTextJump_->text_ = "[SPACE]" + controlTextJump_->text_;
	controlTextAct_->text_ = "[  C  ]" + controlTextAct_->text_;

	controlTextVec_->material_.ambient_ = { 1,1,1 };
	controlTextJump_->material_.ambient_ = { 1,1,1 };
	controlTextAct_->material_.ambient_ = { 1,1,1 };

	if (GetDirectionInput() != ADXVector2{ 0,0 })
	{
		controlTextVec_->material_.ambient_ = { 1,1,0 };
	}
	if (GetInputStatus(jump))
	{
		controlTextJump_->material_.ambient_ = { 1,1,0 };
	}
	if (GetInputStatus(attack))
	{
		controlTextAct_->material_.ambient_ = { 1,1,0 };
	}
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

	gameOverFilter_->renderLayer_ = gameOverFilterLayer;
	gameOverFilter_->material_.ambient_ = { 0,0,0 };
	gameOverFilter_->material_.alpha_ = 0.8f;

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
	dead_->transform_.UpdateMatrix();

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
			if (GetInputStatusTrigger(attack))
			{
				restartAnimationAble_ = true;
			}
			if (GetInputStatusTrigger(jump))
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
			deadParticle_->particles_.back()->moveVec_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.1f;
			float particleScale = ADXUtility::RandomRange(0.3f, 0.9f);
			deadParticle_->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } * 0.3f;
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
				deadParticle_->particles_.back()->moveVec_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.3f;
				float particleScale = ADXUtility::RandomRange(0.3f, 0.9f);
				deadParticle_->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } * 3;
				deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = uiLayer;
			}
			deadParticle_->animation_.delayFrame_ = 2;
			deadParticle_->lifeTime_ = deadParticle_->animation_.GetLength() * 3;
			for (int i = 0; i < 9; i++)
			{
				deadParticle_->Emission();
				deadParticle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
				deadParticle_->particles_.back()->moveVec_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.3f;
				float particleScale = ADXUtility::RandomRange(0.3f, 0.9f);
				deadParticle_->particles_.back()->GetGameObject()->transform_.localScale_ = ADXVector3{ particleScale ,particleScale ,particleScale } * 0.5f;
				deadParticle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
				deadParticle_->particles_.back()->GetGameObject()->renderLayer_ = gameOverLayer;
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
