#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include <vector>
#include <list>
#include "Species.h"
#include "PlayerMini.h"
#include "TutorialArea.h"
#include "ADXCamera.h"

class Player : public Species
{
public:
	Player();
	void Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera);
	bool GetInputStatus(int keyIndex);
	bool GetInputStatusTrigger(int keyIndex);
	bool GetInputStatusRelease(int keyIndex);
	ADXCamera* GetCamera() { return camera; };

private:
	void SpeciesUpdate();
	void Move(float walkSpeed, float jumpPower);
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();

private:
	ADXCamera* camera = nullptr;

	ADXObject nose{};
	ADXObject outOfField{};
	ADXObject tutorialWindow{};

	uint32_t noseImage{};
	uint32_t furImage{};
	uint32_t outOfFieldImage{};

	ADXModel rect{};
	ADXModel playerModel{};

	ADXAudio jumpSE{};
	ADXAudio damageSE{};
	ADXAudio windowOpenSE{};

	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<int> config{};

	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };
	std::list<PlayerMini> minis{};
	int splitInterval = 0;
	bool splitable = true;
	float tutorialWindowExAmount = 0;
	bool windowOpening = false;
	bool windowClosing = false;
};