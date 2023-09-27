#include "Projectile.h"

void Projectile::EnemyUpdate()
{
	if (lifetime <= 0)
	{
		Destroy();
	}
	else
	{
		rigidbody.velocity = direction;
		lifetime--;
	}
}