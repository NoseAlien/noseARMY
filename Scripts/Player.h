#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
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

	ADXObject* nose = nullptr;
	ADXObject* outOfField = nullptr;
	ADXObject* tutorialWindow = nullptr;

	ADXModel rect{};
	ADXModel playerModel{};

	ADXAudio jumpSE{};
	ADXAudio damageSE{};
	ADXAudio windowOpenSE{};

	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<BYTE> config{};

	ADXRigidbody* rigidbody{};

	std::list<PlayerMini*> minis{};
	int splitInterval = 0;
	bool splitable = true;
	float tutorialWindowExAmount = 0;
	bool windowOpening = false;
	bool windowClosing = false;

	float deadAnimationProgress = 0;

	bool windowExtend = false;
	bool isOutOfField = false;

	uint32_t setTutorialImg = 0;

public:
	void Initialize(ADXKeyBoardInput* setKeyboard, std::vector<BYTE> setConfig, ADXCamera* setCamera);
	bool GetInputStatus(int keyIndex);
	bool GetInputStatusTrigger(int keyIndex);
	bool GetInputStatusRelease(int keyIndex);
	ADXCamera* GetCamera() { return camera; };

private:
	void LiveEntitiesInitialize();
	void LiveEntitiesUpdate();
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);
	void DeadUpdate();
	void SafetyPhase();
	void Move(float walkSpeed, float jumpPower);
};