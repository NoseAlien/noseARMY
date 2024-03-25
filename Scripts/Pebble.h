#pragma once
#include "ADXComponent.h"
#include "ADXRigidBody.h"
#include "ADXAudioSource.h"

class Pebble : public ADXComponent
{
private:
	ADXModel rect_{};
	ADXModel shadowModel_{};
	uint32_t billBoardTex_ = 0;
	uint32_t shadowTex_ = 0;

	ADXRigidbody* rigidbody_{};
	ADXObject* billBoard_ = nullptr;
	ADXObject* shadow_ = nullptr;
	ADXAudioSource* hitSE_ = nullptr;

	ADXVector3 prevPos_{};
	ADXVector3 prevVel_{};
	float rotAngle_ = 0;
	bool mute_ = true;

private:
	//初期化
	void UniqueInitialize() final;

	//更新
	void UniqueUpdate() final;

	//描画前処理
	void OnPreRender() final;
};