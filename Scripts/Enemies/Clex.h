#pragma once
#include "Enemies/Enemy.h"

class Clex : public Enemy
{
private:
	enum class phase
	{
		idol,
		attack,
	};

private:
	ADXModel* rect_ = nullptr;
	uint32_t attackTex_ = 0;
	phase phase_{};

	ADXObject* face_ = nullptr;
	ADXObject* body_ = nullptr;
	ADXObject* antennaRig_ = nullptr;
	ADXObject* antenna_ = nullptr;

	float bodyScale_ = 1;
	float antennaAngle_ = 0;

	bool shotted_ = false;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
	void Idol();
	void Attack();
};