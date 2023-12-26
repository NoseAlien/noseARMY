#pragma once

#include "ADXComponent.h"
#include "ADXVector3.h"
#include "ADXObject.h"

//慣性の付いた移動を行うオブジェクトに入れるコンポーネント
class ADXRigidbody : public ADXComponent
{
private:
	struct bool3
	{
		bool x;
		bool y;
		bool z;
	};

public:
	float drag_ = 1;
	ADXVector3 gravity_ = { 0,-1,0 };
	float gravityScale_ = 0;
	bool3 dragAxis_ = { true,true,true };
	ADXVector3 velocity_ = { 0,0,0 };

private:
	ADXVector3 prevPos_{};
	bool initializedThisFrame_ = false;

public:
	//慣性移動
	void VelocityMove();

	//前フレームの座標を取得
	ADXVector3 GetPrevPos() { return prevPos_; };

private:
	//初期化処理
	void UniqueInitialize();

	//更新処理
	void UniqueUpdate();
};