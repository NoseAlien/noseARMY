#pragma once
#include "Enemy.h"
#include "ADXUtility.h"
#include <memory>

//敵の湧き方を指定する際に用いるクラス
class EnemySpawnData
{
public:
	enum enemyType
	{
		eType_Cub_E,
		eType_Clex,
	};

	struct SpawnData
	{
		enemyType specie = eType_Cub_E;
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