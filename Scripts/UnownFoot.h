#pragma once

#include "Crazer_Unown.h"

class Unownfoot : public Enemy
{
	enum class phase
	{
		idol,
		walk,
	};

private:
	phase phase_{};

	ADXVector3 targetPoint_;
	float actProgressSpeed_ = 0;
	float walkSpeed_ = 0;
	
public:
	void PlayWalkPhase(ADXVector3 setTargetPoint, float setActProgressSpeed = 0.1f, float setWalkSpeed = 0.5f);

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void Idol();
	void Walk();
};