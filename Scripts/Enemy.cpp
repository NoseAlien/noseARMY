#include "Enemy.h"
#include "PlayerMini.h"
#include "Player.h"
#include "ADXUtility.h"

void Enemy::Initialize()
{
	enemyModel = ADXModel::LoadModel("model/groundBlock.obj");
	enemyTex = ADXImage::LoadADXImage("battleField.png");

	model = &enemyModel;
	texture = enemyTex;

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

void Enemy::SpeciesUpdate()
{
	rigidbody.drag = 0.8f;
	rigidbody.gravity = { 0,0,0 };
	rigidbody.VelocityMove();

	rigidbody.velocity.y /= 0.8f;
	rigidbody.velocity.y -= 0.015f;

	//Ç±Ç±Ç©ÇÁìGÇ≤Ç∆ÇÃå≈óLèàóù ------

	if (targetDetected && attackProgress <= 0)
	{
		ADXVector3 targetRelativePos = targetPos;
		if (transform.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos, transform.parent_->GetMatWorld());
		}
		cursor = targetRelativePos;
		attackProgress = 1;
	}

	transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress > 0)
	{
		ADXVector3 finalTarget = cursor;
		if (attackProgress > 0.5f)
		{
			finalTarget.y += 6;
			rigidbody.velocity = (finalTarget - transform.localPosition_) * 0.05f;
		}
		else if(attackProgress > 0.2f)
		{
			transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress,0.5f,0.2f,1,0),2) * 3.1415f * 6,
				0 });
			if (attackProgress > 0.25f)
			{
				Species::SetAttackObj({ &colliders[0],this,1 });
			}
		}
	}
	attackProgress = min(max(0,attackProgress - 0.006f),1);

	//Ç±Ç±Ç‹Ç≈ìGÇ≤Ç∆ÇÃå≈óLèàóù ------

	rigidbody.Update(this);

	sortingOrder = 1;

	targetDetected = false;
}

void Enemy::DeadUpdate()
{
	colliders[0].pushBackPriority = -2;

	material.ambient = { 0,0,0 };

	rigidbody.drag = 0.8f;
	rigidbody.VelocityMove();

	rigidbody.velocity.y /= 0.8f;
	rigidbody.velocity.y -= 0.015f;

	rigidbody.Update(this);
}

void Enemy::SpeciesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if(myCol->isTrigger && IsArrive())
	{
		for (auto& objItr : Species::GetSpecies())
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr && !colItr.isTrigger && objItr->GetTeam() != GetTeam())
				{
					targetDetected = true;
					targetPos = objItr->transform.GetWorldPosition();
				}
			}
		}
	}
	else if(!myCol->isTrigger && !IsArrive())
	{
		for (auto& objItr : PlayerMini::GetMinis())
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr)
				{
					transform.SetWorldPosition(transform.GetWorldPosition()
						+ (col->GetGameObject()->transform.GetWorldPosition() - transform.GetWorldPosition()) * 0.1f);
					Species::SetAttackObj({ myCol,objItr->GetParent(),maxHP });
				}
			}
		}
	}
}
