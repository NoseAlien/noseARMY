#include "Enemy.h"

void Enemy::Initialize()
{
	VelocityInitialize();
}

void Enemy::UniqueUpdate()
{
	VelocityMove(0.8f);

	velocity.y /= 0.8f;
	velocity.y -= 0.015f;

	VelocityUpdate();
}

void Enemy::VelocityInitialize()
{
	prevPos = transform.localPosition_;
}

void Enemy::VelocityMove(float drag)
{
	velocity = transform.localPosition_ - prevPos;
	prevPos = transform.localPosition_;

	velocity *= drag;
}

void Enemy::VelocityUpdate()
{
	transform.localPosition_ += velocity;
	transform.UpdateMatrix();
}