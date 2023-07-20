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

	material.ambient = { 1,1,1 };

	if (IsArrive())
	{
		SpeciesUpdate();
	}
	else
	{
		DeadUpdate();
	}

	if (attackHitted)
	{
		material.ambient = { 1,0,0 };
		attackHitted = false;
	}


	S_allSpeciesPtr.push_back(this);
}

void Species::Damage(float damage)
{
	if (IsArrive())
	{
		hpAmount -= damage / maxHP;
		attackHitted = true;
	}
}

void Species::SpeciesUpdate()
{
}

void Species::DeadUpdate()
{
}

void Species::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	for (auto& objItr : GetAttackObj())
	{
		if (col == objItr.col && objItr.attacker->team != team)
		{
			Damage(objItr.power);
		}
	}
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