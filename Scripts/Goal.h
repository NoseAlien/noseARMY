#pragma once

#include "ADXObject.h"

class Goal : public ADXObject
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team = "";
	int32_t sceneTransitionCount = MaxSceneTransitionCount;
	ADXObject clearUI{};
	ADXObject keyUI{};
	ADXModel boxModel{};
	ADXModel rectModel{};

public:
	void Initialize(std::string setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};