#include "Goal.h"
#include "Species.h"
#include "ADXSceneManager.h"

void Goal::Initialize(std::string setTeam)
{
	team = setTeam;
}

void Goal::UniqueUpdate()
{

}

void Goal::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	for (auto& objItr : Species::GetSpecies())
	{
		if (!objItr->colliders.empty() && col == &objItr->colliders[0] && objItr->GetTeam() != team)
		{
			ADXSceneManager::SetSceneNum(1);
		}
	}
}
