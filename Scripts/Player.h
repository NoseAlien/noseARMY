#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include "ADXUtility.h"
#include <vector>
#include <list>
#include "LiveEntity.h"
#include "PlayerMini.h"
#include "TutorialArea.h"
#include "ADXCamera.h"
#include "ADXRigidbody.h"

class Player : public LiveEntity
{
private:
	static const uint32_t maxMinisNum = 20;

private:
	ADXCamera* camera = nullptr;

	ADXObject nose{};
	ADXObject outOfField{};
	ADXObject tutorialWindow{};

	ADXModel rect{};
	ADXModel playerModel{};

	ADXAudio jumpSE{};
	ADXAudio damageSE{};
	ADXAudio windowOpenSE{};

	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<int> config{};

	ADXRigidbody rigidbody{};
	ADXParticleSystem deadParticle;


	std::list<std::unique_ptr<PlayerMini, ADXUtility::NPManager<PlayerMini>>> minis{};
	int splitInterval = 0;
	bool splitable = true;
	float tutorialWindowExAmount = 0;
	bool windowOpening = false;
	bool windowClosing = false;

	float deadAnimationProgress = 0;

public:
	Player();
	void Initialize(ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig, ADXCamera* setCamera);
	bool GetInputStatus(int keyIndex);
	bool GetInputStatusTrigger(int keyIndex);
	bool GetInputStatusRelease(int keyIndex);
	ADXCamera* GetCamera() { return camera; };

private:
	void LiveEntitiesUpdate();
	void DeadUpdate();
	void Move(float walkSpeed, float jumpPower);
};