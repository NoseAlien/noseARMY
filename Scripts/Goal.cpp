#include "Goal.h"
#include "Species.h"
#include "SceneTransition.h"

void Goal::Initialize(std::string setTeam)
{
	team = setTeam;
	sceneTransitionCount = MaxSceneTransitionCount;
}

void Goal::UniqueUpdate()
{
	if (sceneTransitionCount != MaxSceneTransitionCount)
	{
		sceneTransitionCount--;
		if (sceneTransitionCount <= 0)
		{
			SceneTransition::ChangeScene(1);
		}
	}
}

void Goal::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (sceneTransitionCount == MaxSceneTransitionCount)
	{
		for (auto& objItr : Species::GetSpecies())
		{
			if (!objItr->colliders.empty() && col == &objItr->colliders[0] && objItr->GetTeam() == team)
			{
				sceneTransitionCount--;
				return;
			}
		}
	}
}