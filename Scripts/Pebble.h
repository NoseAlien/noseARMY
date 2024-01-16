#pragma once
#include "ADXComponent.h"
#include "ADXRigidBody.h"
#include "ADXAudio.h"

class Pebble : public ADXComponent
{
private:
	ADXModel rect_{};
	uint32_t billBoardTex_ = 0;

	ADXRigidbody* rigidbody_{};
	ADXObject* billBoard_ = nullptr;
	ADXAudio hitSE_{};

	ADXVector3 prevPos_{};
	ADXVector3 prevVel_{};
	float rotAngle_ = 0;

private:
	//初期化
	void UniqueInitialize() final;

	//更新
	void UniqueUpdate() final;

	//描画前処理
	void OnPreRender() final;
};