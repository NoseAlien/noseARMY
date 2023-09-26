#include "BattleFieldBox.h"
#include "LiveEntity.h"

void BattleFieldBox::Initialize(std::vector<SpawnData> setGuarders,
	std::list<std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>>>* setEnemiesPtr, std::string setTeam)
{
	colliders = {};
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().colType_ = box;

	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");
	model = &boxModel;
	texture = ADXImage::LoadADXImage("battleField.png");

	sortingOrder = 2;

	enemySpawnData.SetSpawnList(setGuarders);
	enemiesPtr = setEnemiesPtr;
	team = setTeam;

	animationProgress = 0;
}

void BattleFieldBox::FieldUpdate()
{
	if (awake)
	{
		animationProgress += (1 - animationProgress) * 0.2f;
		transform.modelPosition_ = { 0,-(1 - animationProgress) ,0 };
		transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,(1 - animationProgress) * 3,0 });
		transform.modelScale_ = { animationProgress,animationProgress ,animationProgress };

		isVisible = true;


		for (auto& itr : guardersPtr)
		{
			itr->Update();
		}

		if (battling <= 0)
		{
			animationProgress += (-1 - animationProgress) * 0.2f; 
			if (animationProgress <= 0)
			{
				isActive = false;
			}
		}
		battling--;
	}
	else
	{
		isVisible = false;
	}
}

void BattleFieldBox::FieldOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (!awake)
	{
		for (auto& objItr : LiveEntity::GetLiveEntities())
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr && objItr->GetTeam() != team)
				{
					awake = true;

					guardersPtr = enemySpawnData.Spawn(enemiesPtr, team, &transform);
				}
			}
		}
	}
	else
	{
		for (auto& objItr : guardersPtr)
		{
			for (auto& colItr : objItr->colliders)
			{
				if (col == &colItr && objItr->IsLive())
				{
					battling = 10;
				}
			}
		}
	}
}