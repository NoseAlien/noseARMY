#include "Enemy.h"
#include "PlayerMini.h"
#include "Player.h"
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

	EnemyUpdate();

	targetDetected = false;
}

void Enemy::DeadUpdate()
{
	GetGameObject()->GetComponent<ADXCollider>()->pushBackPriority = -2;

	GetGameObject()->material.ambient = { 0,0,0 };

	rigidbody->drag = 0.8f;
	rigidbody->dragAxis = { true,false,true };
	rigidbody->gravity = { 0,-1,0 };
	rigidbody->gravityScale = 0.015f;

	rigidbody->VelocityMove();

	rigidbody->Update(GetGameObject());
}

void Enemy::LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if(myCol->isTrigger && IsLive())
	{
		for (auto& objItr : LiveEntity::GetLiveEntities())
		{
			for (auto& colItr : objItr->GetGameObject()->GetComponents<ADXCollider>())
			{
				if (col == colItr && !colItr->isTrigger && objItr->IsLive() && objItr->GetTeam() != GetTeam())
				{
					targetDetected = true;
					targetPos = objItr->GetGameObject()->transform.GetWorldPosition();
				}
			}
		}
	}
	else if(!myCol->isTrigger && !IsLive())
	{
		for (auto& objItr : PlayerMini::GetMinis())
		{
			for (auto& colItr : objItr->GetGameObject()->GetComponents<ADXCollider>())
			{
				if (col == colItr)
				{
					GetGameObject()->transform.SetWorldPosition(GetGameObject()->transform.GetWorldPosition()
						+ (col->GetGameObject()->transform.GetWorldPosition() - GetGameObject()->transform.GetWorldPosition()) * 0.1f);
					LiveEntity::SetAttackObj({ myCol,objItr->GetParent(),maxHP });
				}
			}
		}
	}
}
