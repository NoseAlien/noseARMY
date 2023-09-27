#pragma once
#include "Enemy.h"

class Clex : public Enemy
{
private:
	std::list<Enemy*> projectiles{};

private:
	void EnemyUpdate();
};