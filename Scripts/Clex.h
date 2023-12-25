#pragma once
#include "Enemy.h"

class Clex : public Enemy
{
private:
	ADXModel rect_{};
	uint32_t attackTex = 0;

	ADXObject* face = nullptr;
	ADXObject* body_ = nullptr;
	ADXObject* antennaRig = nullptr;
	ADXObject* antenna = nullptr;
	std::list<Enemy*> projectiles{};

	float bodyScale = 1;
	float antennaAngle = 0;

	bool shotted = false;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};