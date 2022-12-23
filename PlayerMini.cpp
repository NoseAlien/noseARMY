#include "PlayerMini.h"
#include "Player.h"

PlayerMini::PlayerMini()
{

}

void PlayerMini::Initialize(Player* setParent)
{
	parent = setParent;
	VelocityInitialize();
	for (int i = 0; i < colliders.size(); i++)
	{
		colliders[i].Initialize(this);
	}
}

void PlayerMini::Move(float walkSpeed, float jumpPower)
{
	if (parent->GetInputStatus(0))
	{
		velocity.z += walkSpeed;
	}
	if (parent->GetInputStatus(1))
	{
		velocity.z -= walkSpeed;
	}
	if (parent->GetInputStatus(2))
	{
		velocity.x += walkSpeed;
	}
	if (parent->GetInputStatus(3))
	{
		velocity.x -= walkSpeed;
	}

	if (parent->GetInputStatusTrigger(4))
	{
		velocity.y = jumpPower;
	}
	if (parent->GetInputStatusRelease(4) && velocity.y > 0)
	{
		velocity.y *= 0.2;
	}
}

void PlayerMini::VelocityInitialize()
{
	prevPos = transform.translation_;
}

void PlayerMini::VelocityMove(float drag)
{
	velocity = transform.translation_ - prevPos;
	prevPos = transform.translation_;

	velocity *= drag;
}

void PlayerMini::VelocityUpdate()
{
	transform.translation_ += velocity;
	transform.UpdateMatrix();
}

void PlayerMini::UniqueUpdate()
{
	VelocityMove(0.8f);

	velocity.y /= 0.8f;
	velocity.y -= 0.03f;

	Move(0.1f, 0.8f);

	VelocityUpdate();
}