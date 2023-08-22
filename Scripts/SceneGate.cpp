#include "SceneGate.h"
#include "Species.h"
#include "SceneTransition.h"
#include "ADXCollider.h"

void SceneGate::Initialize(std::string setTeam)
{
	texture = ADXImage::LoadADXImage("goalField.png");
	sortingOrder = 1;

	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().colType_ = box;

	team = setTeam;

	rectModel = ADXModel::CreateRect();
	boxModel = ADXModel::LoadModel("model/battleBox.obj");

	model = &boxModel;

	keyUI.Initialize();
	keyUI.transform.rectTransform = true;
	keyUI.model = &rectModel;
	keyUI.texture = ADXImage::LoadADXImage("PRESS_SPACE.png");
	keyUI.renderLayer = 5;
}

void SceneGate::UniqueUpdate()
{
	keyUI.transform.localPosition_ = { 0,-0.5f + sin(clock() * 0.001f - 1) * 0.02f,0 };
	keyUI.transform.localScale_ = { keyUI.transform.localScale_.x,0.45f,1 };

	if (hitted)
	{
		keyUI.transform.localScale_.x += (0.45f / ADXWindow::GetAspect() - keyUI.transform.localScale_.x) * 0.3f;
		if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_SPACE))
		{
			SceneTransition::ChangeScene(3);
		}

		hitted = false;
	}
	else
	{
		keyUI.transform.localScale_.x = 0;
	}
	keyUI.Update();
}

void SceneGate::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	for (auto& objItr : Species::GetSpecies())
	{
		if (!objItr->colliders.empty() && col == &objItr->colliders[0] && objItr->GetTeam() == team)
		{
			hitted = true;
			return;
		}
	}
}