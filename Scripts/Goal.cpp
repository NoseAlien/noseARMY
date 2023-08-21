#include "Goal.h"
#include "Species.h"
#include "SceneTransition.h"

void Goal::Initialize(std::string setTeam)
{
	texture = ADXImage::LoadADXImage("goalField.png");
	sortingOrder = 1;

	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().colType_ = box;

	team = setTeam;
	sceneTransitionCount = MaxSceneTransitionCount;

	rectModel = ADXModel::CreateRect();
	boxModel = ADXModel::LoadModel("model/battleBox.obj");

	model = &boxModel;

	clearUI.Initialize();
	clearUI.transform.rectTransform = true;
	clearUI.model = &rectModel;
	clearUI.texture = ADXImage::LoadADXImage("clear.png");
	clearUI.renderLayer = 5;

	keyUI = ADXObject::Duplicate(clearUI);
	keyUI.texture = ADXImage::LoadADXImage("PRESS_SPACE.png");
}

void Goal::UniqueUpdate()
{
	clearUI.transform.localPosition_ = { 0,0.5f + sin(clock() * 0.001f) * 0.02f,0 };
	clearUI.transform.localScale_ = { clearUI.transform.localScale_.x,0.5f * ADXWindow::GetAspect(),0.5f };

	keyUI.transform.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI.transform.localScale_ = { keyUI.transform.localScale_.x,0.3f * ADXWindow::GetAspect(),0.3f };

	if (sceneTransitionCount != MaxSceneTransitionCount)
	{
		clearUI.transform.localScale_.x += (0.5f - clearUI.transform.localScale_.x) * 0.3f;
		clearUI.transform.UpdateMatrix();
		clearUI.Update();
		keyUI.Update();

		sceneTransitionCount--;
		if (sceneTransitionCount <= 0)
		{
			keyUI.transform.localScale_.x += (0.3f - keyUI.transform.localScale_.x) * 0.3f;
			if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
			{
				SceneTransition::ChangeScene(2);
			}
		}
		else
		{
			keyUI.transform.localScale_.x = 0;
		}
	}
	else
	{
		clearUI.transform.localScale_.x = 0;
		keyUI.transform.localScale_.x = 0;
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