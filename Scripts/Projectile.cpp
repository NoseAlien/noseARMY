#include "Projectile.h"
#include "ADXCamera.h"

void Projectile::SetData(const ADXVector3& setDirection, uint32_t setVisual)
{
	direction = setDirection;
	visual = setVisual;
}

void Projectile::EnemyInitialize()
{
	rect = ADXModel::CreateRect();

	GetGameObject()->material.alpha = 0;

	billBoard = ADXObject::Create();
	billBoard->transform.parent_ = &GetGameObject()->transform;
	billBoard->model = &rect;
	billBoard->texture = visual;

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
		rigidbody->velocity = direction;
		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger)
			{
				LiveEntity::SetAttackObj({ itr,this,10 });
			}
		}
		lifeTime--;
	}
}

void Projectile::LiveEntitiesOnPreRender()
{
	billBoard->transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldRotation());
}

void Projectile::LiveEntityOnDestroy()
{
	billBoard->Destroy();
}
