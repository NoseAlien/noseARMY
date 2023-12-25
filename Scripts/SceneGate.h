#pragma once

#include "ADXObject.h"

class SceneGate : public ADXComponent
{
public:
	const int32_t MaxSceneTransitionCount = 60;

private:
	std::string team_ = "";
	std::string stageName_ = "";
	bool hitted_ = false;
	ADXObject* keyUI_ = nullptr;
	ADXObject* stageNameUI_ = nullptr;
	ADXModel boxModel_{};
	ADXModel rectModel_{};

public:
	void Initialize(const std::string& team, const std::string& stageName);
	std::string GetStageName() { return stageName_; };

private:
	void UniqueUpdate();
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
};