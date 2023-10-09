#pragma once

#include "LiveEntity.h"
#include "ADXRigidbody.h"

class Enemy : public LiveEntity
{
protected:
	ADXRigidbody rigidbody{};

	ADXModel enemyModel{};
	ADXVector3 targetPos{};
	bool targetDetected = false;
	ADXVector3 cursor{};
	float attackProgress = 0;

public:
	void Initialize();

protected:
	virtual void EnemyInitialize() {};
	virtual void EnemyUpdate() {};

private:
	void LiveEntitiesInitialize();
	void LiveEntitiesUpdate();
	void DeadUpdate();
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};