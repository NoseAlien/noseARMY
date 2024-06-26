﻿#include "PlayerMini.h"
#include "Player.h"
#include "ShadowRenderer.h"

const float aimSpeed = 0.3f;
const int32_t defaultSortingOrder = 2;
const int32_t shadowSortingOrder = 1;

const int32_t PlayerMini::minisCollideLayer = 3;

void PlayerMini::Initialize(Player* setParent)
{
	parent_ = setParent;
}

void PlayerMini::Move(float walkSpeed, float jumpPower)
{
	ADXVector3 cameraRight = parent_->GetCamera()->GetGameObject()->transform_.TransformPointOnlyRotation({1,0,0});
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = parent_->GetCamera()->GetGameObject()->transform_.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y_ = 0;
	cameraForward = cameraForward.Normalize();

	ADXVector2 inputVec = parent_->GetDirectionInput();
	rigidbody_->velocity_ += (cameraRight * inputVec.x_ + cameraForward * inputVec.y_) * walkSpeed;

	if (inputVec != ADXVector2{ 0,0 })
	{
		targetRot_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody_->velocity_.x_, rigidbody_->velocity_.z_),0 });
	}

	if (parent_->GetInputStatusTrigger(Player::jump))
	{
		rigidbody_->velocity_.y_ = jumpPower;
	}
	if (parent_->GetInputStatusRelease(Player::jump) && rigidbody_->velocity_.y_ > 0)
	{
		rigidbody_->velocity_.y_ *= 0.2f;
	}
}

void PlayerMini::UniqueInitialize()
{
	rect_ = ADXModel::CreateRect();
	shadowModel_ = ADXModel::LoadADXModel("model/cylinder.obj");
	shadowTex_ = ADXImage::LoadADXImage("texture/whiteDot.png");

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
	tempCol->pushBackPriority_ = -1;
	tempCol->collideLayer_ = minisCollideLayer;

	tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->radius_ = 1.5f;

	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();

	nose_ = ADXObject::Create({ 0,0,0.7f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose_->transform_.parent_ = &GetGameObject()->transform_;
	ADXModelRenderer* tempRenderer = nose_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	nose_->sortingOrder_ = defaultSortingOrder;

	body_ = ADXObject::Create();
	body_->transform_.parent_ = &GetGameObject()->transform_;
	tempRenderer = body_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/apEGopTIon_fur.png");
	body_->sortingOrder_ = defaultSortingOrder;

	shadow_ = ADXObject::Create();
	shadow_->transform_.localPosition_ = { 0,-6.5f,0 };
	shadow_->transform_.localScale_ = { 1,6,1 };
	shadow_->transform_.parent_ = &GetGameObject()->transform_;
	ShadowRenderer* tempShadowRenderer = shadow_->AddComponent<ShadowRenderer>();
	tempShadowRenderer->model_ = &shadowModel_;
	tempShadowRenderer->texture_ = shadowTex_;
	tempShadowRenderer->material_.ambient_ = { 0,0,0 };
	tempShadowRenderer->material_.alpha_ = 0.6f;
	shadow_->sortingOrder_ = shadowSortingOrder;

	targetRot_ = GetGameObject()->transform_.localRotation_;
}

void PlayerMini::UniqueUpdate()
{
	rigidbody_->drag_ = 0.8f;

	rigidbody_->VelocityMove();

	rigidbody_->dragAxis_.y = false;

	if (parent_->GetInputStatus(Player::attack))
	{
		Move(0.1f, 0.8f);
		rigidbody_->gravityScale_ = 0.02f;
	}
	else
	{
		Move(0.05f, 0.4f);
		rigidbody_->gravityScale_ = 0.01f;
	}

	GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot_, aimSpeed);

	bodyRotAngle_ = fmodf(bodyRotAngle_ + 0.01f + rigidbody_->velocity_.Length(), ADXUtility::Pi);
}

void PlayerMini::OnPreRender()
{
	body_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	body_->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,bodyRotAngle_ });
}
