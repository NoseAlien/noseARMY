#include "SceneGate.h"
#include "Species.h"
#include "SceneTransition.h"

void SceneGate::Initialize(std::string setTeam)
{
	team = setTeam;
	sceneTransitionCount = MaxSceneTransitionCount;

	rect = ADXModel::CreateRect();
}

void SceneGate::UniqueUpdate()
{
}

void SceneGate::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (sceneTransitionCount == MaxSceneTransitionCount)
	{
		for (auto& objItr : Species::GetSpecies())
		{
			if (!objItr->colliders.empty() && col == &objItr->colliders[0] && objItr->GetTeam() == team)
			{
				if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_C))
				{
					SceneTransition::ChangeScene(3);
				}
				return;
			}
		}
	}
}