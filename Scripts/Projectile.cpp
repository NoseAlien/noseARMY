﻿#include "Projectile.h"
#include "ADXCamera.h"

void Projectile::SetData(const ADXVector3& setDirection, uint32_t setVisual, uint32_t setLifeTime, float setAttackPower)
{
	direction_ = setDirection;
	billBoardTex_ = setVisual;
	lifeTime_ = setLifeTime;
	attackPower_ = setAttackPower;
}

void Projectile::EnemyInitialize()
{
	rect_ = ADXModel::CreateRect();

	GetGameObject()->material_.alpha_ = 0;

	visual_->model_ = nullptr;

	//ビルボード用のモデルを作成
	billBoard_ = ADXObject::Create();
	billBoard_->transform_.parent_ = &visual_->transform_;
	billBoard_->model_ = &rect_;
	billBoard_->texture_ = billBoardTex_;
	//体の一部として登録
	bodyParts_.push_back(billBoard_);
}

void Projectile::EnemyUpdate()
{
	if (lifeTime_ <= 0)
	{
		GetGameObject()->Destroy();
	}
	else
	{
		rigidbody_->velocity_ = direction_;
		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
			{
				//当たったらダメージを受けるオブジェクトとして登録
				LiveEntity::SetAttackObj({ itr,this,attackPower_ });
			}
		}
		lifeTime_--;
	}
}

void Projectile::LiveEntitiesOnPreRender()
{
	//ビルボード
	billBoard_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
}