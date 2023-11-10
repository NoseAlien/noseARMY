#include "PlayerMini.h"
#include "Player.h"

void PlayerMini::Initialize(Player* setParent)
{
	parent = setParent;
}

void PlayerMini::Move(float walkSpeed, float jumpPower)
{
	ADXVector3 cameraRight = parent->GetCamera()->GetGameObject()->transform.TransformPointOnlyRotation({1,0,0});
	cameraRight = cameraRight.Normalize();
	ADXVector3 cameraForward = parent->GetCamera()->GetGameObject()->transform.TransformPointOnlyRotation({ 0,0,1 });
	cameraForward.y = 0;
	cameraForward = cameraForward.Normalize();

	if (parent->GetInputStatus(0) || parent->GetInputStatus(1) || parent->GetInputStatus(2) || parent->GetInputStatus(3))
	{
		if (parent->GetInputStatus(0))
		{
			rigidbody->velocity += cameraForward * walkSpeed;
		}
		if (parent->GetInputStatus(1))
		{
			rigidbody->velocity -= cameraForward * walkSpeed;
		}
		if (parent->GetInputStatus(2))
		{
			rigidbody->velocity += cameraRight * walkSpeed;
		}
		if (parent->GetInputStatus(3))
		{
			rigidbody->velocity -= cameraRight * walkSpeed;
		}
		GetGameObject()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(rigidbody->velocity.x, rigidbody->velocity.z),0 });
	}

	if (parent->GetInputStatusTrigger(4))
	{
		rigidbody->velocity.y = jumpPower;
	}
	if (parent->GetInputStatusRelease(4) && rigidbody->velocity.y > 0)
	{
		rigidbody->velocity.y *= 0.2f;
	}
}

void PlayerMini::UniqueInitialize()
{
	rect = ADXModel::CreateRect();

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
	tempCol->pushBackPriority = -1;

	tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->radius_ = 1.1f;

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();

	nose = ADXObject::Create({ 0,0,0.7f }, ADXQuaternion::EulerToQuaternion({ 0,ADXUtility::Pi,0 }), { 0.42f,0.35f,0.35f });
	nose->transform.parent_ = &GetGameObject()->transform;
	nose->model = &rect;
	nose->texture = ADXImage::LoadADXImage("texture/apEGnoSE.png");
	nose->material = GetGameObject()->material;

	body = ADXObject::Create();
	body->transform.parent_ = &GetGameObject()->transform;
	body->model = &rect;
	body->texture = ADXImage::LoadADXImage("texture/apEGopTIon_fur.png");
	body->material = GetGameObject()->material;
}

void PlayerMini::UniqueUpdate()
{
	rigidbody->drag = 0.8f;

	rigidbody->VelocityMove();

	rigidbody->dragAxis.y = false;

	if (parent->GetInputStatus(5))
	{
		Move(0.1f, 0.8f);
		rigidbody->gravityScale = 0.02f;
	}
	else
	{
		Move(0.05f, 0.4f);
		rigidbody->gravityScale = 0.01f;
	}

	bodyRotAngle = fmodf(bodyRotAngle + 0.01f + rigidbody->velocity.Length(), ADXUtility::Pi);
}

void PlayerMini::OnPreRender()
{
	body->transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldRotation());
	body->transform.UpdateMatrix();
	body->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,bodyRotAngle });
}
