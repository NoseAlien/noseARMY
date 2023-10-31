#pragma once

#include "LiveEntity.h"
#include "ADXRigidbody.h"
#include "PlayerMini.h"

class Enemy : public LiveEntity
{
private:
	const uint32_t maxCarcassLifeTime = 1800;

private:
	int32_t carcassLifeTime = maxCarcassLifeTime;

protected:
	ADXRigidbody* rigidbody;

	ADXModel enemyModel{};
	uint32_t nutralTex = 0;
	uint32_t deadTex = 0;

	ADXVector3 targetPos{};
	bool targetDetected = false;
	ADXVector3 cursor{};
	float attackProgress = 0;

	PlayerMini* grabber = nullptr;

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