#pragma once
#include "Enemy.h"
#include "Projectile.h"

class Projectile : public Enemy
{
private:
	ADXModel rect_{};
	std::vector<uint32_t> texs_ = {};
	float texIndex_ = 0;
	float animProgressSpeed_ = 0;

	LiveEntity* parent_ = nullptr;
	ADXObject* billBoard_ = nullptr;
	uint32_t maxLifeTime_ = 0;
	int32_t lifeTime_ = 0;
	float attackPower_ = 0;

	ADXVector3 direction_ = { 0,0,0 };

public:
	void SetData(const ADXVector3& setDirection, std::vector<uint32_t> setTexs, float setAnimProgressSpeed = 1, uint32_t setLifeTime = 30, float setAttackPower = 20);

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};