#pragma once

#include "ADXObject.h"
#include <string>

struct AttackObject
{
	ADXCollider* col = nullptr;
	float power = 0;
};

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

public:
	static void StaticUpdate();

private:
	static std::vector<Species*> S_species;
	static std::vector<Species*> S_allSpeciesPtr;
	static std::vector<AttackObject> S_attackObjs;
	static std::vector<AttackObject> S_allAttackObj;

public:
	static void SetAttackObj(AttackObject attackObj);
	static std::vector<AttackObject> GetAttackObj() { return S_attackObjs; };
	static std::vector<Species*> GetSpecies() { return S_species; };
};