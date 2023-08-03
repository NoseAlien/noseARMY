#include "ADXParticle.h"
#include "ADXCamera.h"

void ADXParticle::Initialize()
{
	lifeTime = maxLifeTime;
}

void ADXParticle::UniqueUpdate()
{
	transform.localPosition_ += moveVec;
	animation.Update();
	texture = animation.GetNowTex();
	lifeTime--;
}

void ADXParticle::OnPreRender()
{
	if (billboard)
	{
		transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->transform.GetWorldRotation());
	}
}
