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
	std::string team = "";
	ADXModel boxModel{};

public:
	void Initialize(std::vector<SpawnData> setGuarders, std::string setTeam = "");

private:
	void FieldUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};