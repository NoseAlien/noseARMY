#include "Enemy.h"
#include "Player.h"
#include "ADXUtility.h"

void Enemy::Initialize()
{
	enemyModel = ADXModel::LoadADXModel("model/groundBlock.obj");

	model = &enemyModel;
	texture = ADXImage::LoadADXImage("battleField.png");

	damageSE = ADXAudio::LoadADXAudio("sound/slam.wav");
	defeatSE = ADXAudio::LoadADXAudio("sound/slap.wav");

	colliders = {};
	colliders.push_back(ADXCollider(this));
	colliders.back().pushable_ = true;
	colliders.back().colType_ = box;
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().colType_ = sphere;
	colliders.back().radius_ = 12;

	rigidbody.Initialize(this);
}

void Enemy::LiveEntitiesUpdate()
{
	rigidbody.VelocityMove();

	rigidbody.drag = 0.8f;
	rigidbody.dragAxis = { true,false,true };
	rigidbody.gravity = { 0,-1,0 };
	rigidbody.gravityScale = 0.015f;

	EnemyUpdate();

	rigidbody.Update(this);
	targetDetected = false;
}

void Enemy::DeadUpdate()
{
	colliders[0].pushBackPriority = -2;

	material.ambient = { 0,0,0 };

	rigidbody.drag = 0.8f;
	rigidbody.dragAxis = { true,false,true };
	rigidbody.gravity = { 0,-1,0 };
	rigidbody.gravityScale = 0.015f;

	if (grabber != nullptr)
	{
		transform.SetWorldPosition(transform.GetWorldPosition()
			+ (grabber->transform.GetWorldPosition() - transform.GetWorldPosition()) * 0.1f);
		LiveEntity::SetAttackObj({ &colliders[0],grabber->GetParent(),maxHP });
		rigidbody.velocity = { 0,0,0 };
	}

	rigidbody.VelocityMove();

	rigidbody.Update(this);
}

void Enemy::LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if(myCol->isTrigger && IsLive())
	{
		for (auto& objItr : LiveEntity::GetLiveEntities())
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr && !colItr.isTrigger && objItr->IsLive() && objItr->GetTeam() != GetTeam())
				{
					targetDetected = true;
					targetPos = objItr->transform.GetWorldPosition();
				}
			}
		}
	}
	else if(!myCol->isTrigger && !IsLive())
	{
		for (auto& objItr : PlayerMini::GetMinis())
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr)
				{
					grabber = objItr;
				}
			}
		}
	}
}
