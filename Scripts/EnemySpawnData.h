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
	std::vector<SpawnData> spawnList_{};

public:
	void SetSpawnList(const std::vector<SpawnData>& setSpawnList);
	std::list<Enemy*> Spawn(const std::string& team_, ADXWorldTransform* tf = nullptr);
};