#include "ADXParticle.h"
#include "ADXCamera.h"

void ADXParticle::UniqueInitialize()
{
	lifeTime = maxLifeTime;
}

void ADXParticle::UniqueUpdate()
{
	GetGameObject()->transform.localPosition_ += moveVec;
	animation.Update();
	GetGameObject()->texture = animation.GetNowTex();
	lifeTime--;
	if (lifeTime <= 0)
	{
		GetGameObject()->Destroy();
	}
}

void ADXParticle::OnPreRender()
{
	if (billboard)
	{
		GetGameObject()->transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldRotation());
	}
}
