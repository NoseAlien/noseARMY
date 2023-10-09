#include "Projectile.h"

void Projectile::EnemyUpdate()
{
	if (lifetime <= 0)
	{
		GetGameObject()->Destroy();
	}
	else
	{
		rigidbody.velocity = direction;
		lifetime--;
	}
}