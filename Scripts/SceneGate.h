﻿#pragma once

#include "ADXObject.h"

class SceneGate : public ADXComponent
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team = "";
	bool hitted = false;
	ADXObject *keyUI = nullptr;
	ADXModel boxModel{};
	ADXModel rectModel{};

public:
	void Initialize(const std::string& setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};