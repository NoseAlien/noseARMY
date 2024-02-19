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
	
private:
	void EnemyInitialize() final;
	void EnemyUpdate() final;
	void Idol();
	void Walk();
};