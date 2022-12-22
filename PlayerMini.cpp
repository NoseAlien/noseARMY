#include "PlayerMini.h"

PlayerMini::PlayerMini()
{

}

void PlayerMini::Initialize(Player* setParent)
{
	parent = setParent;
	VelocityInitialize();
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
	velocity.y -= 0.015f;

	VelocityUpdate();
}