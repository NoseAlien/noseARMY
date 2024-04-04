#pragma once

#include "LiveEntity.h"
#include "ADXRigidbody.h"
#include "PlayerMini.h"

//敵キャラクターの基底クラス
class Enemy : public LiveEntity
{
private:
	const uint32_t maxCarcassLifeTime = 1800;

protected:
	ADXRigidbody* rigidbody_;

	ADXModel enemyModel_{};
	uint32_t nutralTex_ = 0;
	uint32_t deadTex_ = 0;

	float scale_ = 1;
	ADXVector3 targetPos_{};
	bool targetDetected_ = false;
	ADXVector3 cursor_{};
	float actProgress_ = 0;

	PlayerMini* grabber_ = nullptr;

private:
	int32_t carcassLifeTime_ = maxCarcassLifeTime;

protected:
	//敵ごとの固有初期化処理
	virtual void EnemyInitialize() {};

	//敵ごとの固有更新処理
	virtual void EnemyUpdate() {};

	//敵ごとの固有メモリ管理
	virtual void EnemySafetyPhase() {};

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void LiveEntitiesInitialize() final;

	//生きている時の更新処理
	void LiveEntitiesUpdate() final;

	//死んでいる時の更新処理
	void DeadUpdate() final;

	//何かに触れた時に呼び出される
	void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol);

	//メモリ管理
	void SafetyPhase() final;

	//---以上の関数は必要な時に自動で呼び出される---
};