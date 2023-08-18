#pragma once

#include "ADXObject.h"

class SceneGate : public ADXObject
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team = "";
	int32_t sceneTransitionCount = MaxSceneTransitionCount;
	ADXModel box{};
	ADXModel rect{};

public:
	void Initialize(std::string setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};