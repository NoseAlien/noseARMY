#include "Enemy.h"
#include "PlayerMini.h"
#include "Player.h"

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
	colliders[0].pushBackPriority = -2;

	material.ambient = { 0,0,0 };

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

void Enemy::SpeciesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (IsArrive())
	{
		return;
	}

	for (auto& objItr : PlayerMini::GetMinis())
	{
		for (auto& colItr : objItr->colliders)
		{
			if (col == &colItr)
			{
				transform.SetWorldPosition(col->GetGameObject()->transform.GetWorldPosition());
				Species::SetAttackObj(AttackObject{ myCol,objItr->GetParent(),maxHP});
			}
		}
	}
}
