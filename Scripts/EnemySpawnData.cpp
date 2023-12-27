#include "EnemySpawnData.h"

#include "Cub_E.h"
#include "Clex.h"

void EnemySpawnData::SetSpawnList(const std::vector<SpawnData>& setSpawnList)
{
	if (spawnList_.empty())
	{
		spawnList_ = setSpawnList;
	}
}

std::list<Enemy*> EnemySpawnData::Spawn(const std::string& team, ADXWorldTransform* tf)
{
	std::list<Enemy*> ret = {};

	for (auto& spawnItr : spawnList_)
	{
		ADXObject* temp = ADXObject::Create();

		switch (spawnItr.specie)
		{
		case 1:
			ret.push_back(temp->AddComponent<Cub_E>());
			break;
		case 2:
			ret.push_back(temp->AddComponent<Clex>());
			break;
		default:
			ret.push_back(temp->AddComponent<Enemy>());
			break;
		}

		if (tf == nullptr)
		{
			temp->transform_.localPosition_ = spawnItr.position;
			temp->transform_.localRotation_ = spawnItr.rotation;
		}
		else
		{
			temp->transform_.localPosition_ = ADXMatrix4::Transform(spawnItr.position, tf->GetMatWorld());
			temp->transform_.localRotation_ = tf->TransformRotation(spawnItr.rotation);
		}
		temp->transform_.UpdateMatrix();
		ret.back()->LiveEntity::Initialize(team);

	}

	return ret;
}