#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXGamePadInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include <vector>
#include <list>
#include "LiveEntity.h"
#include "PlayerMini.h"
#include "TutorialArea.h"
#include "ADXCamera.h"
#include "ADXRigidbody.h"


enum actionsList
{
	jump,
	attack
};

struct keyboardConfig
{
	BYTE up = 0;
	BYTE down = 0;
	BYTE right = 0;
	BYTE left = 0;
	BYTE jump = 0;
	BYTE attack = 0;
};
struct gamePadConfig
{
	ControllerButton jump{};
	ControllerButton attack{};
};
class Player : public LiveEntity
{

private:
	static const uint32_t maxMinisNum = 20;

private:
	ADXCamera* camera_ = nullptr;

	ADXObject* nose_ = nullptr;
	ADXObject* outOfField_ = nullptr;
	ADXObject* tutorialWindow_ = nullptr;
	ADXObject* gameOverFilter_ = nullptr;
	ADXObject* dead_ = nullptr;
	ADXObject* keyUI_ = nullptr;

	ADXModel rect_{};
	ADXModel playerModel_{};

	ADXAudio jumpSE_{};
	ADXAudio windowOpenSE_{};

	keyboardConfig keyboardConfig_{};
	gamePadConfig gamePadConfig_{};

	ADXRigidbody* rigidbody_{};

	ADXParticleSystem* deadParticle_ = nullptr;

	std::list<PlayerMini*> minis_{};
	int splitInterval_ = 0;
	bool splitable_ = true;
	float tutorialWindowExAmount_ = 0;
	bool windowOpening_ = false;
	bool windowClosing_ = false;

	float deadAnimationProgress_ = 0;
	bool restartAnimationAble_ = false;
	float restartAnimationProgress_ = 0;

	bool windowExtend_ = false;
	bool isOutOfField_ = false;

	uint32_t setTutorialImg_ = 0;

public:
	void Initialize(const keyboardConfig& setKeyBoardConfig,
		const gamePadConfig& setGamePadConfig,
		ADXCamera* setCamera);
	bool GetInputStatus(actionsList action);
	bool GetInputStatusTrigger(actionsList action);
	bool GetInputStatusRelease(actionsList action);
	ADXVector2 GetDirectionInput();
	ADXCamera* GetCamera() { return camera_; };

private:
	void LiveEntitiesInitialize() final;
	void LiveEntitiesUpdate() final;
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;
	void DeadUpdate() final;
	void SafetyPhase() final;
	void Move(float walkSpeed, float jumpPower);
	void ViewUpdate();
};