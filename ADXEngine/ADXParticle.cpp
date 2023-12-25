#include "ADXParticle.h"
#include "ADXCamera.h"

void ADXParticle::UniqueInitialize()
{
	lifeTime_ = maxLifeTime_;
}

void ADXParticle::UniqueUpdate()
{
	GetGameObject()->transform_.localPosition_ += moveVec_;
	animation_.Update();
	GetGameObject()->texture_ = animation_.GetNowTex();
	lifeTime_--;
	if (lifeTime_ <= 0)
	{
		GetGameObject()->Destroy();
	}
}

void ADXParticle::OnPreRender()
{
	if (billboard_)
	{
		GetGameObject()->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	}
}
