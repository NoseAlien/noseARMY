#include "Projectile.h"
#include "ADXCamera.h"

const float attackPower = 20;

void Projectile::SetData(const ADXVector3& setDirection, uint32_t setVisual)
{
	direction = setDirection;
	billBoardTex = setVisual;
}

void Projectile::EnemyInitialize()
{
	rect_ = ADXModel::CreateRect();

	GetGameObject()->material_.alpha_ = 0;

	visual_->model_ = nullptr;

	//ビルボード用のモデルを作成
	billBoard = ADXObject::Create();
	billBoard->transform_.parent_ = &visual_->transform_;
	billBoard->model_ = &rect_;
	billBoard->texture_ = billBoardTex;
	//体の一部として登録
	bodyParts_.push_back(billBoard);

	lifeTime = maxLifeTime;
}

void Projectile::EnemyUpdate()
{
	if (lifeTime <= 0)
	{
		GetGameObject()->Destroy();
	}
	else
	{
		rigidbody_->velocity_ = direction;
		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
			{
				//当たったらダメージを受けるオブジェクトとして登録
				LiveEntity::SetAttackObj({ itr,this,attackPower });
			}
		}
		lifeTime--;
	}
}

void Projectile::LiveEntitiesOnPreRender()
{
	//ビルボード
	billBoard->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
}