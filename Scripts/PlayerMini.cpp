#include "PlayerMini.h"
#include "Player.h"

void PlayerMini::Initialize(Player* setParent)
{
	parent = setParent;
}

void PlayerMini::Move(float walkSpeed, float jumpPower)
{
	ADXVector3 cameraRight = parent->GetCamera()->GetGameObject()->transform_.TransformPointOnlyRotation({1,0,0});
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = parent->GetCamera()->GetGameObject()->transform_.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y_ = 0;
	cameraForward = cameraForward.Normalize();

	if (parent->GetInputStatus(0) || parent->GetInputStatus(1) || parent->GetInputStatus(2) || parent->GetInputStatus(3))
	{
		if (parent->GetInputStatus(0))
		{
			rigidbody->velocity_ += cameraForward * walkSpeed;
		}
		if (parent->GetInputStatus(1))
		{
			rigidbody->velocity_ -= cameraForward * walkSpeed;
		}
		if (parent->GetInputStatus(2))
		{
			rigidbody->velocity_ += cameraRight * walkSpeed;
		}
		if (parent->GetInputStatus(3))
		{
			rigidbody->velocity_ -= cameraRight * walkSpeed;
		}
		GetGameObject()->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody->velocity_.x_, rigidbody->velocity_.z_),0 });
	}

	if (parent->GetInputStatusTrigger(4))
	{
		rigidbody->velocity_.y_ = jumpPower;
	}
	if (parent->GetInputStatusRelease(4) && rigidbody->velocity_.y_ > 0)
	{
		rigidbody->velocity_.y_ *= 0.2f;
	}
}

void PlayerMini::UniqueInitialize()
{
	rect = ADXModel::CreateRect();

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
	tempCol->pushBackPriority_ = -1;

	tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->radius_ = 1.1f;

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();

	nose = ADXObject::Create({ 0,0,0.7f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose->transform_.parent_ = &GetGameObject()->transform_;
	nose->model_ = &rect;
	nose->texture_ = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	nose->material_ = GetGameObject()->material_;

	body = ADXObject::Create();
	body->transform_.parent_ = &GetGameObject()->transform_;
	body->model_ = &rect;
	body->texture_ = ADXImage::LoadADXImage("texture/apEGopTIon_fur.png");
	body->material_ = GetGameObject()->material_;
}

void PlayerMini::UniqueUpdate()
{
	rigidbody->drag_ = 0.8f;

	rigidbody->VelocityMove();

	rigidbody->dragAxis_.y = false;

	if (parent->GetInputStatus(5))
	{
		Move(0.1f, 0.8f);
		rigidbody->gravityScale_ = 0.02f;
	}
	else
	{
		Move(0.05f, 0.4f);
		rigidbody->gravityScale_ = 0.01f;
	}

	bodyRotAngle = fmodf(bodyRotAngle + 0.01f + rigidbody->velocity_.Length(), ADXUtility::Pi);
}

void PlayerMini::OnPreRender()
{
	body->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	body->transform_.UpdateMatrix();
	body->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,bodyRotAngle });
}
