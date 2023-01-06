#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include <vector>
#include <list>
#include "ADXObject.h"
#include "PlayerMini.h"
#include "TutorialArea.h"
#include "ADXCamera.h"

class Player : public ADXObject
{
public:
	Player();
	void Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera);
	bool GetInputStatus(int keyIndex);
	bool GetInputStatusTrigger(int keyIndex);
	bool GetInputStatusRelease(int keyIndex);
	ADXVector3 GetCameraRight() { return cameraRight; };
	ADXVector3 GetCameraForward() { return cameraForward; };

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
	ADXObject tutorialWindow{};
	ADXCamera* camera = nullptr;

	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };
	ADXVector3 cameraRight{};
	ADXVector3 cameraForward{};
	std::list<PlayerMini> minis{};
	int splitInterval = 0;
	float tutorialWindowExAmount = 0;
};