#include "PlayerMini.h"
#include "Player.h"

void PlayerMini::Initialize(Player* setParent, const ADXObject& setNose)
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
	tempCol->pushBackPriority = -1;

	tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->radius_ = 1.1f;

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();

	parent = setParent;

	nose = ADXObject::Duplicate(setNose);
	nose->transform.parent_ = &GetGameObject()->transform;
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

void PlayerMini::UniqueUpdate()
{
	rigidbody->gravityScale = 0.01f;
	rigidbody->drag = 0.8f;

	rigidbody->VelocityMove();

	rigidbody->velocity.y /= 0.8f;
	rigidbody->velocity.y -= 0.03f;

	Move(0.1f, 0.8f);

	nose->transform.parent_ = &GetGameObject()->transform;
}