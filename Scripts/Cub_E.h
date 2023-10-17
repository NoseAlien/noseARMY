#pragma once
#include "Enemy.h"

class Cub_E : public Enemy
{
private:
	ADXModel rect{};
	uint32_t preAttackTex = 0;
	uint32_t attackTex = 0;

	ADXObject* hair = nullptr;
	ADXObject* tailRig = nullptr;
	ADXObject* tail = nullptr;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};