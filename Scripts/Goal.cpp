#include "Goal.h"
#include "Species.h"
#include "SceneTransition.h"

void Goal::Initialize(std::string setTeam)
{
	team = setTeam;
	sceneTransitionCount = MaxSceneTransitionCount;

	rect = ADXModel::CreateRect();

	clearUI.Initialize();
	clearUI.transform.rectTransform = true;
	clearUI.model = &rect;
	clearUI.texture = ADXImage::LoadADXImage("clear.png");
	clearUI.renderLayer = 5;
}

void Goal::UniqueUpdate()
{
	clearUI.transform.localPosition_ = { 0,0.5f + sin(clock() * 0.001f) * 0.02f,0 };
	clearUI.transform.localScale_ = { clearUI.transform.localScale_.x,0.5f * ADXWindow::GetAspect(),0.5f };
	if (sceneTransitionCount != MaxSceneTransitionCount)
	{
		clearUI.transform.localScale_.x += (0.5f - clearUI.transform.localScale_.x) * 0.3f;
		clearUI.transform.UpdateMatrix();
		clearUI.Update();

		sceneTransitionCount--;
		if (sceneTransitionCount <= 0)
		{
			SceneTransition::ChangeScene(1);
		}
	}
	else
	{
		clearUI.transform.localScale_.x = 0;
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