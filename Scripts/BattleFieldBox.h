#pragma once

#include "FieldBox.h"
#include "Enemy.h"

struct SpawnData
{
	ADXVector3 position{};
	ADXQuaternion rotation{};
};

class BattleFieldBox : public FieldBox
{
private:
	std::vector<SpawnData> guarders{};
	std::list<Enemy> guardersInstance{};
	bool awake = false;
	int32_t battling = 10;
	std::string team = "";
	ADXModel boxModel{};
	float animationProgress = 0;

public:
	void Initialize(std::vector<SpawnData> setGuarders, std::string setTeam = "");

private:
	void FieldUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};