#pragma once
#include "Enemies/Enemy.h"

class Cub_E : public Enemy
{
private:
	enum class phase
	{
		idol,
		attack,
	};

private:
	ADXAudioSource* jumpSE_ = nullptr;

	ADXModel* rect_ = nullptr;
	uint32_t preAttackTex_ = 0;
	uint32_t attackTex_ = 0;
	phase phase_{};
	float prevActProgress_ = 0;

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