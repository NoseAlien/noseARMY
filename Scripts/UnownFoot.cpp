#include "UnownFoot.h"

void Unownfoot::PlayWalkPhase(ADXVector3 setTargetPoint, float setActProgressSpeed, float setWalkSpeed)
{
	targetPoint_ = setTargetPoint;
	actProgressSpeed_ = setActProgressSpeed;
	walkSpeed_ = setWalkSpeed;

	actProgress_ = 1;
	phase_ = phase::walk;
}

void Unownfoot::EnemyInitialize()
{
}

void Unownfoot::EnemyUpdate()
{
	switch (phase_)
	{
	case phase::walk:
		Walk();
		break;
	default:
		Idol();
		break;
	}

	actProgress_ = min(max(0, actProgress_ - actProgressSpeed_), 1);
}

void Unownfoot::Idol()
{
}

void Unownfoot::Walk()
{
	if (actProgress_ > 0)
	{

	}
	else
	{
		phase_ = phase::idol;
	}
}
