#include "ADXParticleSystem.h"

void ADXParticleSystem::Emission()
{
	if (particles.size() < maxParticleNum)
	{
		ADXObject* temp = ADXObject::Create();
		particles.push_back(temp->AddComponent<ADXParticle>());
		ADXParticle* particle = particles.back();
		temp->transform.parent_ = parent;
		temp->transform.UpdateMatrix();
		temp->model = &particleModel;
		particle->animation = animation;
		particle->maxLifeTime = lifeTime;
		particle->billboard = billboard;
	}
}

void ADXParticleSystem::UniqueInitialize()
{
	parent = &GetGameObject()->transform;
}

void ADXParticleSystem::UniqueUpdate()
{
	particles.remove_if([=](auto& itr)
		{ return itr = nullptr; });

	for (auto& itr : particles)
	{
		itr->Update(GetGameObject());
	}
}