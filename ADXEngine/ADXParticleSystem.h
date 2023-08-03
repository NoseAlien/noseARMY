#pragma once
#include "ADXComponent.h"
#include "ADXParticle.h"
#include "ADXTexAnimation.h"
#include <list>

class ADXParticleSystem : public ADXComponent
{
public:
	ADXWorldTransform* parent = nullptr;
	bool loopEmission = false;
	uint32_t burstParticleNum = 1;
	uint32_t coolTime = 0;
	uint32_t maxParticleNum = 100;
	int32_t lifeTime = 10;
	ADXTexAnimation animation{};
	ADXModel particleModel;
	std::list<ADXParticle> particles{};
	bool billboard = true;

private:
	int32_t nowCoolTime = 0;

public:
	void Emission();

private:
	void UniqueInitialize();
	void UniqueUpdate();
};