#pragma once
#include "Enemy.h"

class Clex : public Enemy
{
private:
	ADXModel rect{};
	ADXObject* body = nullptr;
	std::list<Enemy*> projectiles{};

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};