#include "Projectile.h"

void Projectile::EnemyUpdate()
{
	if (lifetime <= 0)
	{
		gameObject->Destroy();
	}
	else
	{
		rigidbody.velocity = direction;
		lifetime--;
	}
}