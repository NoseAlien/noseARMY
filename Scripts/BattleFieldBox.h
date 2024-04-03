#pragma once

#include "FieldBox.h"
#include "Enemies/Enemy.h"
#include "EnemySpawnData.h"

//バトルしないと出られない部屋
class BattleFieldBox : public FieldBox
{
public:
	struct generateData
	{
		ADXVector3 localPosition{};
		ADXQuaternion localRotation{};
		ADXVector3 localScale{};
		std::vector<EnemySpawnData::SpawnData> setGuarders;
		std::string setTeam = "";
	};

private:
	EnemySpawnData enemySpawnData_{};
	std::list<Enemy*> guardersPtr_ = {};
	bool awake_ = false;
	bool guarderSpawned_ = false;
	int32_t battling_ = 10;
	std::string team_ = "";
	ADXModel* boxModel_ = nullptr;
	float animationProgress_ = 0;

public:
	//配置する敵などを設定
	void Initialize(const std::vector<EnemySpawnData::SpawnData>& setGuarders, const std::string& setTeam = "");

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void FieldInitialize();

	//更新処理
	void FieldUpdate();

	//何かに触れた時に呼び出される
	void FieldOnCollisionHit(ADXCollider* col, ADXCollider* myCol);

	//---以上の関数は必要な時に自動で呼び出される---
};