#pragma once

#include "ADXObject.h"

class SceneGate : public ADXComponent
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team_ = "";
	bool hitted_ = false;
	ADXObject *keyUI_ = nullptr;
	ADXModel boxModel_{};
	ADXModel rectModel_{};

public:
	void Initialize(const std::string& setTeam);

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};