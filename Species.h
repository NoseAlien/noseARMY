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
	void Initialize(const std::string& setTeam);
	void UniqueUpdate();

	bool IsArrive() { return hpAmount > 0; };
	std::string GetTeam() { return team; };

protected:
	void Damage(float damage);
	virtual void SpeciesUpdate();
	virtual void DeadUpdate();

private:
	static std::vector<Species*> S_species;
	static std::vector<Species*> S_allSpeciesPtr;
};