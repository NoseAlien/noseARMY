#include "EnemySpawnData.h"

#include "Cub_E.h"
#include "Clex.h"

void EnemySpawnData::SetSpawnList(std::vector<SpawnData> setSpawnList)
{
	if (spawnList.empty())
	{
		spawnList = setSpawnList;
	}
}

std::list<Enemy*> EnemySpawnData::Spawn(std::string team, ADXWorldTransform* tf)
{
	std::list<Enemy*> ret = {};

	for (auto& spawnItr : spawnList)
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
			temp->transform.localPosition_ = spawnItr.position;
			temp->transform.localRotation_ = spawnItr.rotation;
		}
		else
		{
			temp->transform.localPosition_ = ADXMatrix4::Transform(spawnItr.position, tf->GetMatWorld());
			temp->transform.localRotation_ = tf->TransformRotation(spawnItr.rotation);
		}
		temp->transform.UpdateMatrix();
		ret.back()->LiveEntity::Initialize(team);

	}

	return ret;
}