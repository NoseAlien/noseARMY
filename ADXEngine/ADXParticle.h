#pragma once
#include "ADXObject.h"
#include "ADXTexAnimation.h"

class ADXParticle : public ADXComponent
{
public:
	ADXVector3 moveVec{};
	ADXQuaternion rotate{};
	uint32_t maxLifeTime = 0;
	int32_t lifeTime = 0;
	ADXTexAnimation animation{};
	bool billboard = true;

public:
	void Initialize();

private:
	void UniqueUpdate();
	void OnPreRender();
};