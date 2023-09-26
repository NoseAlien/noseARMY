#pragma once
#include "Enemy.h"
#include "ADXUtility.h"
#include <memory>

struct SpawnData
{
	uint32_t specie = 0;
	ADXVector3 position{};
	ADXQuaternion rotation{};
};

class EnemySpawnData
{
private:
	std::vector<SpawnData> spawnList{};

public:
	void SetSpawnList(std::vector<SpawnData> setSpawnList);
	std::list<Enemy*> Spawn(std::list<std::unique_ptr<Enemy, ADXUtility::NPManager<Enemy>>>* enemiesListPtr,
		std::string team = "", ADXWorldTransform* tf = nullptr);
};