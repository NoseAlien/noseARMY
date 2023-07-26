#include "PlayerMini.h"
#include "Player.h"

PlayerMini::PlayerMini()
{

}

void PlayerMini::Initialize(Player* setParent, ADXObject setNose)
{
	colliders.back().pushBackPriority = -1;
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().radius_ = 1.1f;

	parent = setParent;

	nose = Duplicate(setNose);
	nose.transform.parent_ = &transform;

	VelocityInitialize();

	for (int i = 0; i < colliders.size(); i++)
	{
		colliders[i].Initialize(this);
	}
}

void PlayerMini::Move(float walkSpeed, float jumpPower)
{
	if (parent->GetInputStatus(0) || parent->GetInputStatus(1) || parent->GetInputStatus(2) || parent->GetInputStatus(3))
	{
		if (parent->GetInputStatus(0))
		{
			velocity += parent->GetCameraForward() * walkSpeed;
		}
		if (parent->GetInputStatus(1))
		{
			velocity -= parent->GetCameraForward() * walkSpeed;
		}
		if (parent->GetInputStatus(2))
		{
			velocity += parent->GetCameraRight() * walkSpeed;
		}
		if (parent->GetInputStatus(3))
		{
			velocity -= parent->GetCameraRight() * walkSpeed;
		}
		transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,atan2(velocity.x, velocity.z),0 });
	}

	if (parent->GetInputStatusTrigger(4))
	{
		velocity.y = jumpPower;
	}
	if (parent->GetInputStatusRelease(4) && velocity.y > 0)
	{
		velocity.y *= 0.2f;
	}
}

void PlayerMini::VelocityInitialize()
{
	prevPos = transform.localPosition_;
}

void PlayerMini::VelocityMove(float drag)
{
	velocity = transform.localPosition_ - prevPos;
	prevPos = transform.localPosition_;

	velocity *= drag;
}

void PlayerMini::VelocityUpdate()
{
	transform.localPosition_ += velocity;
	transform.UpdateMatrix();
}

void PlayerMini::UniqueUpdate()
{
	VelocityMove(0.8f);

	velocity.y /= 0.8f;
	velocity.y -= 0.03f;

	Move(0.1f, 0.8f);

	VelocityUpdate();

	nose.transform.parent_ = &transform;
	nose.Update();
}