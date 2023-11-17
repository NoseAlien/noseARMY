#include "Enemy.h"
#include "ADXUtility.h"

void Enemy::LiveEntitiesInitialize()
{
	damageSE_ = ADXAudio::LoadADXAudio("sound/knock.wav");
	defeatSE_ = ADXAudio::LoadADXAudio("sound/slap.wav");

	enemyModel_ = ADXModel::LoadADXModel("model/groundBlock.obj");

	visual_->model_ = &enemyModel_;
	visual_->texture_ = ADXImage::LoadADXImage("texture/battleField.png");

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = sphere;
	tempCol->radius_ = 12;

	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();
	EnemyInitialize();
}

void Enemy::LiveEntitiesUpdate()
{
	rigidbody_->VelocityMove();

	rigidbody_->drag_ = 0.8f;
	rigidbody_->dragAxis_ = { true,false,true };
	rigidbody_->gravity_ = { 0,-1,0 };
	rigidbody_->gravityScale_ = 0.015f;

	visual_->texture_ = nutralTex_;

	EnemyUpdate();

	targetDetected_ = false;
	carcassLifeTime_ = maxCarcassLifeTime;
}

void Enemy::DeadUpdate()
{
	rigidbody_->drag_ = 0.8f;
	rigidbody_->dragAxis_ = { true,false,true };
	rigidbody_->gravity_ = { 0,-1,0 };
	rigidbody_->gravityScale_ = 0.015f;
	rigidbody_->VelocityMove();

	for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
	{
		itr->pushBackPriority_ = -2;
	}

	visual_->texture_ = deadTex_;

	if (clock() % 1000 < 100 || (carcassLifeTime_ <= (int32_t)maxCarcassLifeTime / 4 && clock() % 200 < 100))
	{
		visual_->material_.ambient_ = { 0.2f,0.2f,0.2f };
		for (auto& itr : bodyParts_)
		{
			itr->material_.ambient_ = { 0.2f,0.2f,0.2f };
		}
	}

	if (grabber_ != nullptr)
	{
		GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.GetWorldPosition()
			+ (grabber_->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition()) * 0.1f);

		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
			{
				LiveEntity::SetAttackObj({ itr,(LiveEntity*)grabber_->GetParent(),maxHP_ });
			}
		}
	}

	carcassLifeTime_--;
	if (carcassLifeTime_ <= 0)
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
			targetDetected_ = true;
			targetPos_ = col->GetGameObject()->transform_.GetWorldPosition();
		}
	}
	else if(!myCol->isTrigger_ && !IsLive() && grabber_ == nullptr)
	{
		if (col->GetGameObject()->GetComponent<PlayerMini>())
		{
			grabber_ = col->GetGameObject()->GetComponent<PlayerMini>();
		}
		if (!col->isTrigger_ && col->GetGameObject()->GetComponent<Enemy>()
			&& col->GetGameObject()->GetComponent<Enemy>()->grabber_ != nullptr)
		{
			grabber_ = col->GetGameObject()->GetComponent<Enemy>()->grabber_;
		}
	}
}

void Enemy::SafetyPhase()
{
	if (grabber_ != nullptr && grabber_->GetGameObject()->GetDeleteFlag())
	{
		grabber_ = nullptr;
	}
	EnemySafetyPhase();
}