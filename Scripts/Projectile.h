#pragma once
#include "Enemy.h"
#include "Projectile.h"

class Projectile : public Enemy
{
private:
	ADXModel rect_{};
	uint32_t billBoardTex_ = 0;

	LiveEntity* parent_ = nullptr;
	ADXObject* billBoard_ = nullptr;
	uint32_t maxLifeTime_ = 0;
	int32_t lifeTime_ = 0;

	ADXVector3 direction_ = { 0,0,0 };

public:
	void SetData(const ADXVector3& setDirection, uint32_t setVisual, uint32_t setLifeTime = 30);

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};