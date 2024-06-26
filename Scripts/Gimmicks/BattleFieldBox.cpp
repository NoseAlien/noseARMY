﻿#include "Gimmicks/BattleFieldBox.h"
#include "ADXModelRenderer.h"
#include "LiveEntity.h"

const float animationProgressSpeed = 0.2f;
const float animationRotSpeed = 3;

void BattleFieldBox::Initialize(const std::vector<EnemySpawnData::SpawnData>& setGuarders, const std::string& setTeam)
{
	enemySpawnData_.SetSpawnList(setGuarders);
	team_ = setTeam;
}

void BattleFieldBox::FieldInitialize()
{
	boxModel_ = ADXModel::LoadADXModel("model/battleBox.obj");
	ADXModelRenderer* tempRenderer = GetGameObject()->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &boxModel_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/battleField.png");

	GetGameObject()->sortingOrder_ = 3;


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

		animationProgress_ += (1 - animationProgress_) * animationProgressSpeed;
		GetGameObject()->transform_.modelPosition_ = { 0,-(1 - animationProgress_) ,0 };
		GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,(1 - animationProgress_) * animationRotSpeed,0 });
		GetGameObject()->transform_.modelScale_ = { animationProgress_,animationProgress_ ,animationProgress_ };

		GetGameObject()->isVisible_ = true;

		if (battling_ <= 0)
		{
			animationProgress_ += (-1 - animationProgress_) * animationProgressSpeed;
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
		//これのチームIDと違うIDを持ったLiveEntityが入ったら起動
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
					//guardersPtr_の中の敵が触れ続けている限り消えないようにする
					battling_ = 10;
				}
			}
		}
	}
}