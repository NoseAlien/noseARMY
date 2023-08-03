#include "ADXParticle.h"

void ADXParticle::Initialize()
{
	lifeTime = maxLifeTime;
}

void ADXParticle::UniqueUpdate()
{
	transform.localPosition_ += moveVec;
	lifeTime--;

}
