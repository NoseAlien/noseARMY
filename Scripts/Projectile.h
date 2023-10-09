#pragma once
#include "Enemy.h"
#include "Projectile.h"

class Projectile : public Enemy
{
private:
	LiveEntity* parent = nullptr;
	ADXObject* billBoard = nullptr;
	uint32_t billBoardTex = 0;
	uint32_t maxLifeTime = 60;
	int32_t lifetime = 0;

	ADXVector3 direction = { 0,0,0 };

private:
	void EnemyUpdate();
};