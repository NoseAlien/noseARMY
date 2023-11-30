#include "BattleFieldBox.h"
#include "LiveEntity.h"

void BattleFieldBox::Initialize(const std::vector<SpawnData>& setGuarders, const std::string& setTeam)
{
	enemySpawnData_.SetSpawnList(setGuarders);
	team_ = setTeam;
}

void BattleFieldBox::FieldInitialize()
{
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");
	GetGameObject()->model_ = &boxModel_;
	GetGameObject()->texture_ = ADXImage::LoadADXImage("texture/battleField.dds");

	GetGameObject()->sortingOrder_ = 2;

	animationProgress_ = 0;
}

void BattleFieldBox::FieldUpdate()
{
	if (awake_)
	{
		if (!guarderSpawned_)
		{
			guardersPtr_ = enemySpawnData_.Spawn(team_, &GetGameObject()->transform_);
			guarderSpawned_ = true;
		}

		animationProgress_ += (1 - animationProgress_) * 0.2f;
		GetGameObject()->transform_.modelPosition_ = { 0,-(1 - animationProgress_) ,0 };
		GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,(1 - animationProgress_) * 3,0 });
		GetGameObject()->transform_.modelScale_ = { animationProgress_,animationProgress_ ,animationProgress_ };

		GetGameObject()->isVisible_ = true;

		if (battling_ <= 0)
		{
			animationProgress_ += (-1 - animationProgress_) * 0.2f; 
			if (animationProgress_ <= 0)
			{
				GetGameObject()->isActive_ = false;
			}
		}
		battling_--;
	}
	else
	{
		GetGameObject()->isVisible_ = false;
	}
}

void BattleFieldBox::FieldOnCollisionHit(ADXCollider* col, [[maybe_unused]] ADXCollider* myCol)
{
	if (!awake_)
	{
		LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
		if (tempLiv != nullptr && tempLiv->GetTeam() != team_)
		{
			awake_ = true;
		}
	}
	else
	{
		for (auto& objItr : guardersPtr_)
		{
			for (auto& colItr : objItr->GetGameObject()->GetComponents<ADXCollider>())
			{
				if (col == colItr && objItr->IsLive())
				{
					battling_ = 10;
				}
			}
		}
	}
}