#include "BattleFieldBox.h"
#include "LiveEntity.h"

void BattleFieldBox::Initialize(std::vector<SpawnData> setGuarders, std::string setTeam)
{
	enemySpawnData.SetSpawnList(setGuarders);
	team = setTeam;
}

void BattleFieldBox::FieldInitialize()
{
	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");
	GetGameObject()->model = &boxModel;
	GetGameObject()->texture = ADXImage::LoadADXImage("battleField.png");

	GetGameObject()->sortingOrder = 2;

	animationProgress = 0;
}

void BattleFieldBox::FieldUpdate()
{
	if (awake)
	{
		animationProgress += (1 - animationProgress) * 0.2f;
		GetGameObject()->transform.modelPosition_ = { 0,-(1 - animationProgress) ,0 };
		GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,(1 - animationProgress) * 3,0 });
		GetGameObject()->transform.modelScale_ = { animationProgress,animationProgress ,animationProgress };

		GetGameObject()->isVisible = true;

		if (battling <= 0)
		{
			animationProgress += (-1 - animationProgress) * 0.2f; 
			if (animationProgress <= 0)
			{
				GetGameObject()->isActive = false;
			}
		}
		battling--;
	}
	else
	{
		GetGameObject()->isVisible = false;
	}
}

void BattleFieldBox::FieldOnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	if (!awake)
	{
		LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
		if (tempLiv != nullptr && tempLiv->GetTeam() != team)
		{
			awake = true;

			guardersPtr = enemySpawnData.Spawn(team, &GetGameObject()->transform);
		}
	}
	else
	{
		for (auto& objItr : guardersPtr)
		{
			for (auto& colItr : objItr->GetGameObject()->GetComponents<ADXCollider>())
			{
				if (col == colItr && objItr->IsLive())
				{
					battling = 10;
				}
			}
		}
	}
}