#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXGamePadInput.h"
#include "ADXVector3.h"
#include "ADXAudioSource.h"
#include <vector>
#include <list>
#include "LiveEntity.h"
#include "PlayerMini.h"
#include "TutorialArea.h"
#include "ADXCamera.h"
#include "ADXRigidbody.h"
#include "ADXTextRenderer.h"

//自機のオブジェクトに入れるコンポーネント
class Player : public LiveEntity
{
public:
	enum actionsList
	{
		jump,
		attack
	};

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
	ADXObject* deathCountIcon_ = nullptr;
	ADXObject* deathCountUI_ = nullptr;
	ADXObject* killCountIcon_ = nullptr;
	ADXObject* killCountUI_ = nullptr;

	ADXModel rect_{};
	ADXModel playerModel_{};

	ADXAudioSource* jumpSE_ = nullptr;
	ADXAudioSource* splitSE_ = nullptr;
	ADXAudioSource* absorbSE_ = nullptr;
	ADXAudioSource* windowOpenSE_ = nullptr;

	ADXRigidbody* rigidbody_ = nullptr;

	ADXTextRenderer* controlTextVec_ = nullptr;
	ADXTextRenderer* controlTextCam_ = nullptr;
	ADXTextRenderer* controlTextJump_ = nullptr;
	ADXTextRenderer* controlTextAct_ = nullptr;

	ADXParticleSystem* shardParticle_ = nullptr;
	ADXParticleSystem* deadParticle_ = nullptr;

	float scale_ = 1;
	ADXVector2 cameraTiltVelocity_{};
	float cameraHeight_ = 0.3f;
	ADXVector2 prevInputVec_{};
	ADXQuaternion targetRot_{};
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
	uint32_t deathCount_ = 0;

public:
	//引数のアクションに割り当てられたボタンが押されている時にtrueを返す
	bool GetInputStatus(actionsList action);

	//引数のアクションに割り当てられたボタンが押された瞬間にtrueを返す
	bool GetInputStatusTrigger(actionsList action);

	//引数のアクションに割り当てられたボタンを離した瞬間にtrueを返す
	bool GetInputStatusRelease(actionsList action);

	//方向入力の値を返す
	ADXVector2 GetDirectionInput();

	//カメラ制御用の方向入力の値を返す
	ADXVector2 GetCameraControlInput();

	//自機に割り当てられたカメラを返す
	ADXCamera* GetCamera() { return camera_; };

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理、主に必要なオブジェクトの生成
	void LiveEntitiesInitialize() final;

	//生きている時の更新処理
	void LiveEntitiesUpdate() final;

	//何かに触れた時に呼び出される
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;

	//死んでいる時の更新処理
	void DeadUpdate() final;

	//消えそうなインスタンスを参照していたらここで切る
	void SafetyPhase() final;

	//---以上の関数は必要な時に自動で呼び出される---

	//移動
	void Move(float walkSpeed, float jumpPower);

	//カメラの位置、角度を適切な状態にする
	void ViewUpdate();
};