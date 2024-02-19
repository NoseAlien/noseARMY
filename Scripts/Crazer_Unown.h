#pragma once

#include "Enemy.h"

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

	std::list<Enemy*> foots_{};

private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void EnemySafetyPhase() final;
	void Idol();
	void Walk();
	void Shot();
};