#pragma once
#include "Enemy.h"
#include "ADXUtility.h"
#include <memory>

//敵の湧き方を指定する際に用いるクラス
class EnemySpawnData
{
public:
	struct SpawnData
	{
		uint32_t specie = 0;
		ADXVector3 position{};
		ADXQuaternion rotation{};
	};

private:
	std::vector<SpawnData> spawnList_{};

public:
	//敵の湧き方を設定
	void SetSpawnList(const std::vector<SpawnData>& setSpawnList);

	//予め設定されたパターンの通りに敵を生成
	std::list<Enemy*> Spawn(const std::string& team, ADXWorldTransform* tf = nullptr);
};