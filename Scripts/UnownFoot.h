#pragma once

#include "Crazer_Unown.h"

class UnownFoot : public Enemy
{
	enum class phase
	{
		idol,
		walk,
	};

private:
	uint32_t attackTex_ = 0;
	phase phase_{};

	ADXVector3 targetPoint_;
	bool attack_ = false;
	float actProgressSpeed_ = 0;
	float walkSpeed_ = 0;
	
public:
	void PlayWalkPhase(const ADXVector3& setTargetPoint, bool setAttack = false,
		float setActProgressSpeed = 1, float setWalkSpeed = 1);

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void Idol();
	void Walk();
};