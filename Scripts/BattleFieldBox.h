#pragma once

#include "FieldBox.h"
#include "Enemy.h"
#include "EnemySpawnData.h"

class BattleFieldBox : public FieldBox
{
private:
	EnemySpawnData enemySpawnData_{};
	std::list<Enemy*> guardersPtr_ = {};
	bool awake_ = false;
	bool guarderSpawned_ = false;
	int32_t battling_ = 10;
	std::string team_ = "";
	ADXModel boxModel_{};
	float animationProgress_ = 0;

public:
	void Initialize(const std::vector<SpawnData>& setGuarders, const std::string& setTeam = "");

private:
	void FieldInitialize();
	void FieldUpdate();
	void FieldOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};