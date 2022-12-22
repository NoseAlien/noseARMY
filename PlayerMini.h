#pragma once

#include "ADXObject.h"

class Player;

class PlayerMini : public ADXObject
{
public:
	PlayerMini();
	void Initialize(Player* setParent);

private:
	void UniqueUpdate();
	void Move(float walkSpeed, float jumpPower);
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();

private:
	Player* parent = nullptr;
	bool destroyFlag = false;
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };
};