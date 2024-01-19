#pragma once
#include "Enemy.h"

class Cub_E : public Enemy
{
private:
	ADXModel rect_{};
	uint32_t preAttackTex_ = 0;
	uint32_t attackTex_ = 0;

	ADXObject* hair_ = nullptr;
	ADXObject* tailRig_ = nullptr;
	ADXObject* tail_ = nullptr;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
};