#pragma once

#include "ADXObject.h"

class Goal : public ADXObject
{
private:
	std::string team = "";

public:
	void Initialize(std::string setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};