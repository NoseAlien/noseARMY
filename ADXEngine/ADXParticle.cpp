#include "ADXParticle.h"
#include "ADXCamera.h"

void ADXParticle::Initialize()
{
	lifeTime = maxLifeTime;
}

void ADXParticle::UniqueUpdate()
{
	GetGameObject()->transform.localPosition_ += moveVec;
	animation.Update();
	GetGameObject()->texture = animation.GetNowTex();
	lifeTime--;
}

void ADXParticle::OnPreRender()
{
	if (billboard)
	{
		GetGameObject()->transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldRotation());
	}
}
