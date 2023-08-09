#pragma once

#include "ADXObject.h"

class Goal : public ADXObject
{
public:
	const int32_t MaxSceneTransitionCount = 120;

private:
	std::string team = "";
	int32_t sceneTransitionCount = MaxSceneTransitionCount;
	ADXObject clearUI{};
	ADXModel rect{};

public:
	void Initialize(std::string setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};