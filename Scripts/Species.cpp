#include "Species.h"
#include "ADXCamera.h"
#include <time.h>

std::vector<Species*> Species::S_species = {};
std::vector<Species*> Species::S_allSpeciesPtr = {};
std::vector<AttackObject> Species::S_attackObjs = {};
std::vector<AttackObject> Species::S_allAttackObj = {};


void Species::Initialize(const std::string& setTeam)
{
	team = setTeam;

	rect = ADXModel::CreateRect();
	gaugeTex = ADXImage::LoadADXImage("whiteDot.png");

	hpGaugeBG.Initialize();
	hpGaugeBG.transform.parent_ = &transform;
	hpGaugeBG.transform.UpdateMatrix();
	hpGaugeBG.model = &rect;
	hpGaugeBG.texture = gaugeTex;
	hpGaugeBG.renderLayer = 1;

	hpGauge.Initialize();
	hpGauge.transform.parent_ = &hpGaugeBG.transform;
	hpGauge.transform.UpdateMatrix();
	hpGauge.model = &rect;
	hpGauge.texture = gaugeTex;
	hpGauge.renderLayer = 2;
}

void Species::UniqueUpdate()
{
	hpGaugeBG.material.ambient = { 0.1f,0.1f,0.1f };
	hpGauge.material.ambient = { 0.1f,1,0.3f };

	hpGaugeBG.transform.localPosition_ = { 0,-1.5f,0 };
	hpGauge.transform.localPosition_ = { (1 - hpAmount),0,0 };
	hpGauge.transform.localScale_ = { hpAmount,1,1 };

	material.ambient = { 1,1,1 };

	transform.modelPosition_ = { 0,0,0 };

	if (IsArrive())
	{
		SpeciesUpdate();
		hpGaugeBG.transform.localScale_ = { 0.5,0.2f,1 };
	}
	else
	{
		DeadUpdate();
		hpGaugeBG.transform.localScale_ = { 0.5,max(0,hpGaugeBG.transform.localScale_.y - 0.03f),1};
	}

	if (attackHitted)
	{
		material.ambient = { 1,0,0 };
		transform.modelPosition_ = { (rand() % 11 - 5) * 0.02f ,(rand() % 11 - 5) * 0.02f,(rand() % 11 - 5) * 0.02f};
		hpGaugeBG.transform.localPosition_ = { 0,-1.5f + (float)sin(clock()) * 0.05f,0};
		attackHitted = false;
	}

	hpGaugeBG.Update();
	hpGauge.Update();

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

void Species::OnPreRender()
{
	hpGaugeBG.transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->transform.GetWorldRotation());
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