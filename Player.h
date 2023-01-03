#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include <vector>
#include <list>
#include "ADXObject.h"
#include "PlayerMini.h"
#include "ADXCamera.h"

class Player : public ADXObject
{
public:
	Player();
	void Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera);
	bool GetInputStatus(int keyIndex);
	bool GetInputStatusTrigger(int keyIndex);
	bool GetInputStatusRelease(int keyIndex);

private:
	void UniqueUpdate();
	void Move(float walkSpeed, float jumpPower);
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();

private:
	ADXObject nose{};
	ADXImage noseImage{};
	ADXImage furImage{};
	ADXModel rect;
	ADXAudio se{};
	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<int> config{};
	ADXCamera* camera = nullptr;

	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };
	std::list<PlayerMini> minis{};
	int splitInterval = 0;
};