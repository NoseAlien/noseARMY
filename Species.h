#pragma once

#include "ADXObject.h"
#include <string>

struct AttackObject;

class Species : public ADXObject
{
protected:
	float maxHP = 999;
	float hpAmount = 1;

private:
	ADXModel rect{};
	ADXObject hpGauge;
	ADXObject hpGaugeBG;
	std::string team;
	bool attackHitted;

public:
	void Initialize(const std::string& setTeam);
	void UniqueUpdate();

	bool IsArrive() { return hpAmount > 0; };
	std::string GetTeam() { return team; };

protected:
	void Damage(float damage);
	virtual void SpeciesUpdate();
	virtual void DeadUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);

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

struct AttackObject
{
	ADXCollider* col = nullptr;
	Species* attacker = nullptr;
	float power = 0;
};