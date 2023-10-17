﻿#include "Enemy.h"
#include "ADXUtility.h"

void Enemy::LiveEntitiesInitialize()
{
	enemyModel = ADXModel::LoadADXModel("model/groundBlock.obj");

	GetGameObject()->model = &enemyModel;
	GetGameObject()->texture = ADXImage::LoadADXImage("battleField.png");

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = sphere;
	tempCol->radius_ = 12;

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();
	EnemyInitialize();
}

void Enemy::LiveEntitiesUpdate()
{
	rigidbody->VelocityMove();

	rigidbody->drag = 0.8f;
	rigidbody->dragAxis = { true,false,true };
	rigidbody->gravity = { 0,-1,0 };
	rigidbody->gravityScale = 0.015f;

	GetGameObject()->texture = nutralTex;

	EnemyUpdate();

	targetDetected = false;
}

void Enemy::DeadUpdate()
{
	GetGameObject()->GetComponent<ADXCollider>()->pushBackPriority = -2;

	rigidbody->drag = 0.8f;
	rigidbody->dragAxis = { true,false,true };
	rigidbody->gravity = { 0,-1,0 };
	rigidbody->gravityScale = 0.015f;

	for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
	{
		itr->pushBackPriority = -1;
	}

	GetGameObject()->texture = deadTex;

	if (grabber != nullptr)
	{
		GetGameObject()->transform.SetWorldPosition(GetGameObject()->transform.GetWorldPosition()
			+ (grabber->GetGameObject()->transform.GetWorldPosition() - GetGameObject()->transform.GetWorldPosition()) * 0.1f);
		LiveEntity::SetAttackObj({ GetGameObject()->GetComponent<ADXCollider>(),grabber->GetGameObject()->GetComponent<PlayerMini>()->GetParent(),maxHP});
	}

	rigidbody->VelocityMove();
}

void Enemy::LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if(myCol->isTrigger && IsLive())
	{
		LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
		if (!col->isTrigger && tempLiv != nullptr && tempLiv->IsLive() && tempLiv->GetTeam() != GetTeam())
		{
			targetDetected = true;
			targetPos = col->GetGameObject()->transform.GetWorldPosition();
		}
	}
	else if(!myCol->isTrigger && !IsLive() && grabber == nullptr && col->GetGameObject()->GetComponent<PlayerMini>())
	{
		grabber = col->GetGameObject()->GetComponent<PlayerMini>();
	}
}

void Enemy::SafetyPhase()
{
	if (grabber != nullptr && grabber->GetGameObject()->GetDeleteFlag())
	{
		grabber = nullptr;
	}
	EnemySafetyPhase();
}