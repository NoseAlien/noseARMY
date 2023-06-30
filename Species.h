#pragma once

#include "ADXObject.h"
#include <string>

class Species : public ADXObject
{
protected:
	float maxHP = 1;
	float hpAmount = 1;

private:
	ADXModel rect{};
	ADXObject hpGauge;
	ADXObject hpGaugeBG;
	std::string team;

public:
	void Initialize(std::string setTeam);
	void UniqueUpdate();

protected:
	void Damage(float damage);
	virtual void SpeciesUpdate();
};