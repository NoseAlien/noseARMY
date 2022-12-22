#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include <vector>
#include <list>
#include "ADXObject.h"

class PlayerMini;

class Player : public ADXObject
{
public:
	Player();
	void Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig);

protected:
	void Move(float walkSpeed, float jumpPower);
	void VelocityMove(float drag, float dropSpeed);

private:
	void UniqueUpdate();

protected:
	ADXAudio se{};
	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<int> config{};
	bool prevJump = false;
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };
	std::list<ADXObject> minis{};
};