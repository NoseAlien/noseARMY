#pragma once

#include "Species.h"
#include "ADXRigidbody.h"

class Enemy : public Species
{
protected:
	ADXRigidbody rigidbody{};

	ADXModel enemyModel{};
	uint32_t enemyTex{};
	ADXVector3 targetPos{};
	bool targetDetected = false;

private:
	//�G�ŗL�X�e�[�^�X
	ADXVector3 cursor{};
	float attackProgress = 0;

public:
	void Initialize();

private:
	void SpeciesUpdate();
	void DeadUpdate();

private:
	void SpeciesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};
