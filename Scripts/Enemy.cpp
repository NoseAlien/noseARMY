#include "Enemy.h"
#include "ADXUtility.h"

void Enemy::LiveEntitiesInitialize()
{
	damageSE = ADXAudio::LoadADXAudio("sound/hit.wav");
	defeatSE = ADXAudio::LoadADXAudio("sound/slap.wav");

	enemyModel = ADXModel::LoadADXModel("model/groundBlock.obj");

	visual->model = &enemyModel;
	visual->texture = ADXImage::LoadADXImage("texture/battleField.png");

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

	visual->texture = nutralTex;

	EnemyUpdate();

	targetDetected = false;
	carcassLifeTime = maxCarcassLifeTime;
}

void Enemy::DeadUpdate()
{
	rigidbody->drag = 0.8f;
	rigidbody->dragAxis = { true,false,true };
	rigidbody->gravity = { 0,-1,0 };
	rigidbody->gravityScale = 0.015f;
	rigidbody->VelocityMove();

	for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
	{
		itr->pushBackPriority = -2;
	}

	visual->texture = deadTex;

	if (clock() % 1000 < 100 || (carcassLifeTime <= (int32_t)maxCarcassLifeTime / 4 && clock() % 200 < 100))
	{
		visual->material.ambient = {0.2f,0.2f,0.2f};
	}

	if (grabber != nullptr)
	{
		GetGameObject()->transform.SetWorldPosition(GetGameObject()->transform.GetWorldPosition()
			+ (grabber->GetGameObject()->transform.GetWorldPosition() - GetGameObject()->transform.GetWorldPosition()) * 0.1f);

		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger)
			{
				LiveEntity::SetAttackObj({ itr,(LiveEntity*)grabber->GetParent(),maxHP });
			}
		}
	}

	carcassLifeTime--;
	if (carcassLifeTime <= 0)
	{
		GetGameObject()->Destroy();
	}
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
	else if(!myCol->isTrigger && !IsLive() && grabber == nullptr)
	{
		if (col->GetGameObject()->GetComponent<PlayerMini>())
		{
			grabber = col->GetGameObject()->GetComponent<PlayerMini>();
		}
		if (!col->isTrigger && col->GetGameObject()->GetComponent<Enemy>()
			&& col->GetGameObject()->GetComponent<Enemy>()->grabber != nullptr)
		{
			grabber = col->GetGameObject()->GetComponent<Enemy>()->grabber;
		}
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