#pragma once

#include "ADXObject.h"

class Goal : public ADXComponent
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team_ = "";
	int32_t sceneTransitionCount_ = MaxSceneTransitionCount;
	ADXObject *clearUI_ = nullptr;
	ADXObject *keyUI_ = nullptr;
	ADXModel boxModel_{};
	ADXModel rectModel_{};

public:
	void Initialize(const std::string& setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};