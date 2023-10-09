#pragma once

#include "FieldBox.h"
#include "Enemy.h"
#include "EnemySpawnData.h"

class BattleFieldBox : public FieldBox
{
private:
	EnemySpawnData enemySpawnData{};
	std::list<Enemy*> guardersPtr = {};
	bool awake = false;
	int32_t battling = 10;
	std::string team = "";
	ADXModel boxModel{};
	float animationProgress = 0;

public:
	void Initialize(std::vector<SpawnData> setGuarders, std::string setTeam = "");

private:
	void FieldUpdate();
	void FieldOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};