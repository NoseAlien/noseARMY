#pragma once

#include "LiveEntity.h"
#include "ADXRigidbody.h"
#include "PlayerMini.h"

class Enemy : public LiveEntity
{
private:
	const uint32_t maxCarcassLifeTime = 1800;

protected:
	ADXRigidbody* rigidbody_;

	ADXModel enemyModel_{};
	uint32_t nutralTex_ = 0;
	uint32_t deadTex_ = 0;

	ADXVector3 targetPos_{};
	bool targetDetected_ = false;
	ADXVector3 cursor_{};
	float attackProgress_ = 0;

	PlayerMini* grabber_ = nullptr;

private:
	int32_t carcassLifeTime_ = maxCarcassLifeTime;

protected:
	virtual void EnemyInitialize() {};
	virtual void EnemyUpdate() {};
	virtual void EnemySafetyPhase() {};

private:
	void LiveEntitiesInitialize() final;
	void LiveEntitiesUpdate() final;
	void DeadUpdate() final;
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
	void SafetyPhase() final;
};