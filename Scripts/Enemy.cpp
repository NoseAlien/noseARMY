#include "Enemy.h"

void Enemy::Initialize()
{
	VelocityInitialize();
}

void Enemy::SpeciesUpdate()
{
	VelocityMove(0.8f);

	velocity.y /= 0.8f;
	velocity.y -= 0.015f;

	VelocityUpdate();

	sortingOrder = 1;
}

void Enemy::DeadUpdate()
{
	material.ambient = { 0,0,0 };

	colliders.back().pushBackPriority = -2;

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