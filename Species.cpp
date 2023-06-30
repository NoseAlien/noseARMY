#include "Species.h"
#include "ADXCamera.h"

void Species::Initialize(std::string setTeam)
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
	SpeciesUpdate();
}

void Species::Damage(float damage)
{
	hpAmount -= damage / maxHP;
}

void Species::SpeciesUpdate()
{
}
