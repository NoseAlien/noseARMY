#pragma once
#include "ADXObject.h"
#include "ADXTexAnimation.h"

class ADXParticle : public ADXComponent
{
public:
	ADXVector3 moveVec_{};
	ADXQuaternion rotate_{};
	uint32_t maxLifeTime_ = 0;
	int32_t lifeTime_ = 0;
	ADXTexAnimation animation_{};
	bool billboard_ = true;

private:
	void UniqueInitialize();
	void UniqueUpdate();
	void OnPreRender();
};