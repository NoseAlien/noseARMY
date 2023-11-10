#pragma once
#include "ADXComponent.h"
#include "ADXParticle.h"
#include "ADXTexAnimation.h"
#include <list>

class ADXParticleSystem : public ADXComponent
{
public:
	ADXWorldTransform* parent_ = nullptr;
	bool loopEmission_ = false;
	uint32_t burstParticleNum_ = 1;
	uint32_t coolTime_ = 0;
	uint32_t maxParticleNum_ = 100;
	int32_t lifeTime_ = 10;
	ADXTexAnimation animation_{};
	ADXModel particleModel_{};
	std::list<ADXParticle*> particles_{};
	bool billboard_ = true;

private:
	int32_t nowCoolTime_ = 0;

public:
	void Emission();

private:
	void UniqueInitialize();
	void SafetyPhase();
};