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
	ADXObject* gameOverFilter = nullptr;
	ADXObject* dead = nullptr;
	ADXObject* keyUI = nullptr;

	ADXModel rect{};
	ADXModel playerModel{};

	ADXAudio jumpSE{};
	ADXAudio windowOpenSE{};

	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<BYTE> config{};

	ADXRigidbody* rigidbody{};

	ADXParticleSystem* deadParticle = nullptr;

	std::list<PlayerMini*> minis{};
	int splitInterval = 0;
	bool splitable = true;
	float tutorialWindowExAmount = 0;
	bool windowOpening = false;
	bool windowClosing = false;

	float deadAnimationProgress = 0;
	bool restartAnimationAble = false;
	float restartAnimationProgress = 0;

	bool windowExtend = false;
	bool isOutOfField = false;

	uint32_t setTutorialImg = 0;

public:
	void Initialize(ADXKeyBoardInput* setKeyboard, const std::vector<BYTE>& setConfig, ADXCamera* setCamera);
	bool GetInputStatus(int keyIndex);
	bool GetInputStatusTrigger(int keyIndex);
	bool GetInputStatusRelease(int keyIndex);
	ADXCamera* GetCamera() { return camera; };

private:
	void LiveEntitiesInitialize() final;
	void LiveEntitiesUpdate() final;
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;
	void DeadUpdate() final;
	void SafetyPhase() final;
	void Move(float walkSpeed, float jumpPower);
	void ViewUpdate();
};