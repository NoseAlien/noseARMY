#include "BattleFieldBox.h"
#include "Species.h"

void BattleFieldBox::Initialize(std::vector<SpawnData> setGuarders, std::string setTeam)
{
	battleFieldImg = ADXImage::LoadADXImage("battleField.png");
	texture = battleFieldImg;

	guarders = setGuarders;
	team = setTeam;
}

void BattleFieldBox::FieldUpdate()
{
	if (awake)
	{
		isVisible = true;
		if (guarders.empty())
		{
			isActive = false;
		}
	}
	else
	{
		isVisible = false;
	}
}

void BattleFieldBox::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (!awake)
	{
		for (auto& objItr : Species::GetSpecies())
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr && objItr->GetTeam() != team)
				{
					awake = true;

					for (auto& spawnItr : guarders)
					{

					}
				}
			}
		}
	}
}