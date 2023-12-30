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

		//設定されている種類と同じ敵を生成
		switch (spawnItr.specie)
		{
		case eType_Cub_E:
			ret.push_back(temp->AddComponent<Cub_E>());
			break;
		case eType_Clex:
			ret.push_back(temp->AddComponent<Clex>());
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
		temp->transform_.UpdateMatrix();
		//引数に入れた配列に追加
		ret.back()->LiveEntity::Initialize(team);
	}

	return ret;
}