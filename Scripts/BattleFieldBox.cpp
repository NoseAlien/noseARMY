#include "BattleFieldBox.h"
#include "LiveEntity.h"

void BattleFieldBox::Initialize(std::vector<SpawnData> setGuarders, std::string setTeam)
{
	colliders = {};
	colliders.push_back(ADXCollider(this));
	colliders.back().isTrigger = true;
	colliders.back().colType_ = box;

	boxModel = ADXModel::LoadADXModel("model/battleBox.obj");
	model = &boxModel;
	texture = ADXImage::LoadADXImage("battleField.png");

	sortingOrder = 2;

	guarders = setGuarders;
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


		for (auto& itr : guardersInstance)
		{
			itr.Update();
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

void BattleFieldBox::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
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

					for (auto& spawnItr : guarders)
					{
						guardersInstance.push_back(Enemy());
						guardersInstance.back().ADXObject::Initialize();
						guardersInstance.back().transform.localPosition_ = ADXMatrix4::Transform(spawnItr.position,transform.GetMatWorld());
						guardersInstance.back().transform.localRotation_ = transform.TransformRotation(spawnItr.rotation);
						guardersInstance.back().transform.localScale_ = { 1,1,1 };
						guardersInstance.back().transform.UpdateMatrix();
						guardersInstance.back().Initialize();
						guardersInstance.back().LiveEntity::Initialize(team);
					}
				}
			}
		}
	}
	else
	{
		for (auto& objItr : guardersInstance)
		{
			for (auto& colItr : objItr.colliders)
			{
				if (col == &colItr && objItr.IsLive())
				{
					battling = 10;
				}
			}
		}
	}
}