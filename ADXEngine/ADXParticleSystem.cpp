#include "ADXParticleSystem.h"

void ADXParticleSystem::Emission()
{
	if (particles.size() < maxParticleNum)
	{
		particles.push_back(ADXParticle());
		ADXParticle& particle = particles.back();
		particle.ADXObject::Initialize();
		particle.Initialize();
		particle.transform.parent_ = parent;
		particle.transform.UpdateMatrix();
		particle.model = &particleModel;
		particle.animation = animation;
	}
}

void ADXParticleSystem::UniqueInitialize()
{
	parent = &gameObject->transform;
}

void ADXParticleSystem::UniqueUpdate()
{
	particles.remove_if([=](auto& itr)
		{ return itr.lifeTime <= 0; });

	for (auto& itr : particles)
	{
		itr.Update();
	}
}