#pragma once

#include "Species.h"
#include "ADXRigidbody.h"

class Enemy : public Species
{
protected:
	ADXRigidbody rigidbody{};

	ADXModel enemyModel{};
	ADXVector3 targetPos{};
	bool targetDetected = false;

private:
	//敵固有ステータス
	ADXVector3 cursor{};
	float attackProgress = 0;

public:
	void Initialize();

protected:
	virtual void EnemyUpdate();

private:
	void SpeciesUpdate();
	void DeadUpdate();
	void SpeciesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};
