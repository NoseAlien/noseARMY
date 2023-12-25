#pragma once
#include "Enemy.h"
#include "Projectile.h"

class Projectile : public Enemy
{
private:
	ADXModel rect_{};
	uint32_t billBoardTex = 0;

	LiveEntity* parent_ = nullptr;
	ADXObject* billBoard = nullptr;
	uint32_t maxLifeTime = 30;
	int32_t lifeTime = 0;

	ADXVector3 direction = { 0,0,0 };

public:
	void SetData(const ADXVector3& setDirection, uint32_t setVisual);

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};