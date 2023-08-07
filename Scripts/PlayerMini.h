#pragma once

#include "ADXObject.h"

class Player;

class PlayerMini : public ADXObject
{
public:
	PlayerMini();
	void Initialize(Player* setParent, ADXObject setNose);

private:
	void UniqueUpdate();
	void Move(float walkSpeed, float jumpPower);
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();

private:
	ADXObject nose{};
	Player* parent = nullptr;

	bool destroyFlag = false;
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };

private:
	static std::vector<PlayerMini*> S_minis;
	static std::vector<PlayerMini*> S_allMiniPtr;

public:
	static void StaticUpdate();
	static std::vector<PlayerMini*> GetMinis() { return S_minis; };
};