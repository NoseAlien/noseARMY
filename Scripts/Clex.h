#pragma once
#include "Enemy.h"

class Clex : public Enemy
{
private:
	ADXModel rect_{};
	uint32_t attackTex_ = 0;

	ADXObject* face_ = nullptr;
	ADXObject* body_ = nullptr;
	ADXObject* antennaRig_ = nullptr;
	ADXObject* antenna_ = nullptr;
	std::list<Enemy*> projectiles_{};

	float bodyScale_ = 1;
	float antennaAngle_ = 0;

	bool shotted_ = false;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};