#include "Enemy.h"
#include "ADXUtility.h"

void Enemy::LiveEntitiesInitialize()
{
	damageSE = ADXAudio::LoadADXAudio("sound/hit.wav");
	defeatSE = ADXAudio::LoadADXAudio("sound/slap.wav");

	enemyModel = ADXModel::LoadADXModel("model/groundBlock.obj");

	visual->model_ = &enemyModel;
	visual->texture_ = ADXImage::LoadADXImage("texture/battleField.png");

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = sphere;
	tempCol->radius_ = 12;

	rigidbody = GetGameObject()->AddComponent<ADXRigidbody>();
	EnemyInitialize();
}

void Enemy::LiveEntitiesUpdate()
{
	rigidbody->VelocityMove();

	rigidbody->drag_ = 0.8f;
	rigidbody->dragAxis_ = { true,false,true };
	rigidbody->gravity_ = { 0,-1,0 };
	rigidbody->gravityScale_ = 0.015f;

	visual->texture_ = nutralTex;

	EnemyUpdate();

	targetDetected = false;
	carcassLifeTime = maxCarcassLifeTime;
}

void Enemy::DeadUpdate()
{
	rigidbody->drag_ = 0.8f;
	rigidbody->dragAxis_ = { true,false,true };
	rigidbody->gravity_ = { 0,-1,0 };
	rigidbody->gravityScale_ = 0.015f;
	rigidbody->VelocityMove();

	for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
	{
		itr->pushBackPriority_ = -2;
	}

	visual->texture_ = deadTex;

	if (clock() % 1000 < 100 || (carcassLifeTime <= (int32_t)maxCarcassLifeTime / 4 && clock() % 200 < 100))
	{
		visual->material_.ambient_ = { 0.2f,0.2f,0.2f };
		for (auto& itr : bodyParts)
		{
			itr->material_.ambient_ = { 0.2f,0.2f,0.2f };
		}
	}

	if (grabber != nullptr)
	{
		GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.GetWorldPosition()
			+ (grabber->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition()) * 0.1f);

		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
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
	if(myCol->isTrigger_ && IsLive())
	{
		LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
		if (!col->isTrigger_ && tempLiv != nullptr && tempLiv->IsLive() && tempLiv->GetTeam() != GetTeam())
		{
			targetDetected = true;
			targetPos = col->GetGameObject()->transform_.GetWorldPosition();
		}
	}
	else if(!myCol->isTrigger_ && !IsLive() && grabber == nullptr)
	{
		if (col->GetGameObject()->GetComponent<PlayerMini>())
		{
			grabber = col->GetGameObject()->GetComponent<PlayerMini>();
		}
		if (!col->isTrigger_ && col->GetGameObject()->GetComponent<Enemy>()
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