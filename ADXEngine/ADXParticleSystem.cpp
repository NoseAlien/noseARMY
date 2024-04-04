#include "ADXParticleSystem.h"
#include "ADXModelRenderer.h"

void ADXParticleSystem::Emission()
{
	//パーティクルの最大数に達していなければ
	if (particles_.size() < maxParticleNum_)
	{
		//パーティクル生成
		ADXObject* temp = ADXObject::Create();
		particles_.push_back(temp->AddComponent<ADXParticle>());
		ADXParticle* particle_ = particles_.back();
		//設定を反映
		temp->transform_.parent_ = parent_;
		ADXModelRenderer* tempRenderer = temp->AddComponent<ADXModelRenderer>();
		tempRenderer->model_ = &particleModel_;
		particle_->animation_ = animation_;
		particle_->scale_ = scale_;
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