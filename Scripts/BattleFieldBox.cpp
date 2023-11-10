#include "BattleFieldBox.h"
#include "LiveEntity.h"

void BattleFieldBox::Initialize(const std::vector<SpawnData>& setGuarders, const std::string& setTeam)
{
	enemySpawnData.SetSpawnList(setGuarders);
	team = setTeam;
}

void BattleFieldBox::FieldInitialize()
{
	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");
	GetGameObject()->model_ = &boxModel;
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/battleField.png");

	GetGameObject()->sortingOrder_ = 2;

	animationProgress = 0;
}

void BattleFieldBox::FieldUpdate()
{
	if (awake)
	{
		if (!guarderSpawned)
		{
			guardersPtr = enemySpawnData.Spawn(team, &GetGameObject()->transform_);
			guarderSpawned = true;
		}

		animationProgress += (1 - animationProgress) * 0.2f;
		GetGameObject()->transform_.modelPosition_ = { 0,-(1 - animationProgress) ,0 };
		GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,(1 - animationProgress) * 3,0 });
		GetGameObject()->transform_.modelScale_ = { animationProgress,animationProgress ,animationProgress };

		GetGameObject()->isVisible_ = true;

		if (battling <= 0)
		{
			animationProgress += (-1 - animationProgress) * 0.2f; 
			if (animationProgress <= 0)
			{
				GetGameObject()->isActive_ = false;
			}
		}
		battling--;
	}
	else
	{
		GetGameObject()->isVisible_ = false;
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