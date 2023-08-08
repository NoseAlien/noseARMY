#pragma once

#include "ADXObject.h"

class Player;

class PlayerMini : public ADXObject
{
private:
	ADXObject nose{};
	Player* parent = nullptr;

	bool destroyFlag = false;
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };

public:
	PlayerMini();
	void Initialize(Player* setParent, ADXObject setNose);
	Player* GetParent() { return parent; };
		
private:
	void UniqueUpdate();
	void Move(float walkSpeed, float jumpPower);
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();

private:
	static std::vector<PlayerMini*> S_minis;
	static std::vector<PlayerMini*> S_allMiniPtr;

public:
	static void StaticUpdate();
	static std::vector<PlayerMini*> GetMinis() { return S_minis; };
};