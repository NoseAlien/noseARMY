#include "Species.h"
#include "ADXCamera.h"

std::vector<Species*> Species::S_species = {};
std::vector<Species*> Species::S_allSpeciesPtr = {};
std::vector<AttackObject> Species::S_attackObjs = {};
std::vector<AttackObject> Species::S_allAttackObj = {};


void Species::Initialize(const std::string& setTeam)
{
	team = setTeam;

	hpGaugeBG.Initialize();
	hpGaugeBG.transform.rectTransform = true;
	hpGaugeBG.transform.UpdateMatrix();
	hpGaugeBG.model = &rect;

	hpGauge.Initialize();
	hpGauge.transform.rectTransform = true;
	hpGauge.transform.parent_ = &hpGaugeBG.transform;
	hpGauge.transform.UpdateMatrix();
	hpGauge.model = &rect;

}

void Species::UniqueUpdate()
{
	//hpGaugeBG.transform.localPosition_ = ADXMatrix::transform(transform.GetWorldPosition(), ADXCamera::GetMatView());
	hpGauge.transform.localPosition_ = { (1 - hpAmount) / 2,0,0 };
	hpGauge.transform.localScale_ = { hpAmount,0,0 };
	hpGaugeBG.transform.UpdateMatrix();
	hpGauge.transform.UpdateMatrix();
	if (IsArrive())
	{
		SpeciesUpdate();
	}
	else
	{
		DeadUpdate();
	}
	S_allSpeciesPtr.push_back(this);
}

void Species::Damage(float damage)
{
	hpAmount -= damage / maxHP;
}

void Species::SpeciesUpdate()
{
}

void Species::DeadUpdate()
{
}

void Species::StaticUpdate()
{
	S_species = S_allSpeciesPtr;
	S_allSpeciesPtr.clear();
	S_attackObjs = S_allAttackObj;
	S_allAttackObj.clear();
}

void Species::SetAttackObj(AttackObject attackObj)
{
	S_allAttackObj.push_back(attackObj);
}