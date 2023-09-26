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

std::list<Enemy*> EnemySpawnData::Spawn(std::list<std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>>>* enemiesListPtr, std::string team, ADXWorldTransform* tf)
{
	std::list<Enemy*> ret = {};

	for (auto& spawnItr : spawnList)
	{
		std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>> temp = nullptr;

		switch (spawnItr.specie)
		{
		case 1:
			temp = std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>>(new Cub_E);
			break;
		case 2:
			temp = std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>>(new Clex);
			break;
		default:
			temp = std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>>(new Enemy);
			break;
		}

		enemiesListPtr->push_back(move(temp));
		enemiesListPtr->back()->ADXObject::Initialize();
		if (tf == nullptr)
		{
			enemiesListPtr->back()->transform.localPosition_ = spawnItr.position;
			enemiesListPtr->back()->transform.localRotation_ = spawnItr.rotation;
		}
		else
		{
			enemiesListPtr->back()->transform.localPosition_ = ADXMatrix4::Transform(spawnItr.position, tf->GetMatWorld());
			enemiesListPtr->back()->transform.localRotation_ = tf->TransformRotation(spawnItr.rotation);
		}
		enemiesListPtr->back()->transform.UpdateMatrix();
		enemiesListPtr->back()->Initialize();
		enemiesListPtr->back()->LiveEntity::Initialize(team);

		ret.push_back(enemiesListPtr->back().get());
	}

	return ret;
}