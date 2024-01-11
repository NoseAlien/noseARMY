#pragma once

#include "ADXObject.h"
#include "ADXAudio.h"
#include "ADXParticleSystem.h"
#include "FieldBox.h"
#include <string>

struct AttackObject;

//体力があり、攻撃に当たるとダメージを受けるものの基底クラス
class LiveEntity : public ADXComponent
{
public:
	static const float basicHP;
	static const float basicAttackPower;
	static const float repairPower;
	static const uint32_t maxRepairCoolTime;
	static const uint32_t basicGhostTimeFrame;
	static const uint32_t reviveGhostTimeFrame;

public:
	struct AttackObject
	{
		ADXCollider* col = nullptr;
		LiveEntity* attacker = nullptr;
		float power = 0;
	};

protected:
	float maxHP_ = basicHP;
	float hpAmount_ = 1;
	int32_t ghostTime_ = 0;
	ADXAudio damageSE_{};
	ADXAudio defeatSE_{};
	ADXObject* visual_ = nullptr;
	std::vector<ADXObject*> bodyParts_{};

private:
	ADXModel rect_{};
	uint32_t gaugeTex_ = 0;
	ADXParticleSystem* particle_ = nullptr;
	ADXObject* hpGauge_ = nullptr;
	ADXObject* hpGaugeBG_ = nullptr;
	std::string team_ = "";
	bool attackHitted_ = false;

	uint32_t killCount_ = 0;
	int32_t repairCoolTime_ = 0;
	FieldBox* latestHitField_ = nullptr;
	bool isOutOfField_ = false;

public:
	//チームIDを設定
	void Initialize(const std::string& setTeam);

	//体力が0でなければtrueを返す
	bool IsLive() { return hpAmount_ > 0; }

	//チームIDを取得
	std::string GetTeam() { return team_; }

	//倒した敵の数を取得
	uint32_t GetKillCount() { return killCount_; }

protected:
	//ダメージを受ける
	void Damage(float damage);

	//もし死んでいたら復活する
	void Revive();

	//LiveEntity毎の固有初期化処理
	virtual void LiveEntitiesInitialize() {}

	//生きている時の更新処理
	virtual void LiveEntitiesUpdate() {}

	//LiveEntity毎の固有描画前処理
	virtual void LiveEntitiesOnPreRender() {}

	//LiveEntity毎の固有削除前処理
	virtual void LiveEntityOnDestroy() {}

	//死んでいる時の更新処理
	virtual void DeadUpdate() {}

	//LiveEntity毎の固有接触処理
	virtual void LiveEntitiesOnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {}

	//何かに触れた時に呼び出される
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;

	//描画前処理
	void OnPreRender() final;

private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void UniqueInitialize() final;

	//更新処理
	void UniqueUpdate() final;

	//---以上の関数は必要な時に自動で呼び出される---

private:
	static std::vector<AttackObject> S_attackObjs;
	static std::vector<AttackObject> S_allAttackObj;

public:
	//クラスの更新処理
	static void StaticUpdate();

	//攻撃に用いるコライダーを設定
	static void SetAttackObj(const AttackObject& attackObj);

	//攻撃に用いるコライダーを全て取得
	static std::vector<AttackObject> GetAttackObj() { return S_attackObjs; }
};