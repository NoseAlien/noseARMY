#pragma once
#include "ADXObject.h"
#include "ADXTexAnimation.h"

class ADXParticle : public ADXObject
{
public:
	ADXVector3 moveVec{};
	uint32_t maxLifeTime{};
	int32_t lifeTime;
	ADXTexAnimation animation{};

public:
	void Initialize();

private:
	void UniqueUpdate();
};