#pragma once
#include "Enemy.h"

class Cub_E : public Enemy
{
private:
	uint32_t preAttackTex = 0;
	uint32_t attackTex = 0;

private:
	void EnemyInitialize();
	void EnemyUpdate();
};