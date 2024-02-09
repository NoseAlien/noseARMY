#pragma once
#include "Enemy.h"

class Cub_E : public Enemy
{
private:
	enum class phase
	{
		idol,
		attack,
	};

private:
	ADXModel rect_{};
	uint32_t preAttackTex_ = 0;
	uint32_t attackTex_ = 0;
	phase phase_{};

	ADXObject* hair_ = nullptr;
	ADXObject* tailRig_ = nullptr;
	ADXObject* tail_ = nullptr;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void LiveEntitiesOnPreRender() final;
	void Idol();
	void Attack();
};