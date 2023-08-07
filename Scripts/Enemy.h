#pragma once

#include "Species.h"

class Enemy : public Species
{
protected:
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };

public:
	void Initialize();

private:
	void SpeciesUpdate();
	void DeadUpdate();
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();

private:
	void SpeciesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};
