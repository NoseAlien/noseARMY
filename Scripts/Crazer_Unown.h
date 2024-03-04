#pragma once

#include "Enemy.h"

class UnownFoot;
class Crazer_Unown : public Enemy
{
	enum class phase
	{
		idol,
		walk,
		shot,
	};

private:
	phase phase_{};

	std::vector<uint32_t> attackTexs_ = {};
	float attackTexIndex_ = 0;

	std::list<UnownFoot*> foots_{};
	bool prevWalkOddIndexFoot_ = false;
	int shotInterval_ = 0;

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void EnemySafetyPhase() final;
	void Idol();
	void Walk();
	void Shot();
};