#include "Projectile.h"
#include "ADXCamera.h"

void Projectile::SetData(const ADXVector3& setDirection, uint32_t setVisual)
{
	direction = setDirection;
	billBoardTex = setVisual;
}

void Projectile::EnemyInitialize()
{
	rect = ADXModel::CreateRect();

	GetGameObject()->material_.alpha_ = 0;

	visual->model_ = nullptr;

	billBoard = ADXObject::Create();
	billBoard->transform_.parent_ = &visual->transform_;
	billBoard->model_ = &rect;
	billBoard->texture_ = billBoardTex;
	bodyParts.push_back(billBoard);

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
		rigidbody->velocity_ = direction;
		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
			{
				LiveEntity::SetAttackObj({ itr,this,10 });
			}
		}
		lifeTime--;
	}
}

void Projectile::LiveEntitiesOnPreRender()
{
	billBoard->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
}