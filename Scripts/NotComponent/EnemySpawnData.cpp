#include "NotComponent/EnemySpawnData.h"

#include "Enemies/Cub_E.h"
#include "Enemies/Clex.h"
#include "Enemies/Crazer_Unown.h"

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

		//設定されている種類と同じ敵を生成
		switch (spawnItr.specie)
		{
		case enemyType::Cub_E:
			ret.push_back(temp->AddComponent<Cub_E>());
			break;
		case enemyType::Clex:
			ret.push_back(temp->AddComponent<Clex>());
			break;
		case enemyType::Crazer_Unown:
			ret.push_back(temp->AddComponent<Crazer_Unown>());
			break;
		default:
			ret.push_back(temp->AddComponent<Enemy>());
			break;
		}

		//位置を設定と同じに
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
		//引数に入れた配列に追加
		ret.back()->SetTeam(team);
	}

	return ret;
}