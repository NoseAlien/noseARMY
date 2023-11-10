#include "ADXParticleSystem.h"

void ADXParticleSystem::Emission()
{
	if (particles_.size() < maxParticleNum_)
	{
		ADXObject* temp = ADXObject::Create();
		particles_.push_back(temp->AddComponent<ADXParticle>());
		ADXParticle* particle_ = particles_.back();
		temp->transform_.parent_ = parent_;
		temp->transform_.UpdateMatrix();
		temp->model_ = &particleModel_;
		particle_->animation_ = animation_;
		particle_->maxLifeTime_ = lifeTime_;
		particle_->billboard_ = billboard_;
	}
}

void ADXParticleSystem::UniqueInitialize()
{
	parent_ = &GetGameObject()->transform_;
}

void ADXParticleSystem::SafetyPhase()
{
	particles_.remove_if([=](auto& itr)
		{ return itr->GetGameObject()->GetDeleteFlag(); });
}