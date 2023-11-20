#pragma once

#include "ADXObject.h"
#include "ADXAudio.h"
#include "ADXParticleSystem.h"
#include "FieldBox.h"
#include <string>

struct AttackObject;

class LiveEntity : public ADXComponent
{
public:
	static const float basicHP;
	static const float basicAttackPower;
	static const uint32_t basicGhostTimeFrame;

protected:
	float maxHP_ = basicHP;
	float hpAmount_ = 1;
	int32_t GhostTime_ = 0;
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

	FieldBox* latestHitField_ = nullptr;
	bool isOutOfField_ = false;

private:
	static std::vector<AttackObject> S_attackObjs;
	static std::vector<AttackObject> S_allAttackObj;

public:
	void Initialize(const std::string& setTeam);
	bool IsLive() { return hpAmount_ > 0; };
	std::string GetTeam() { return team_; };

protected:
	void Damage(float damage);
	virtual void LiveEntitiesInitialize() {};
	virtual void LiveEntitiesUpdate() {};
	virtual void LiveEntitiesOnPreRender() {};
	virtual void LiveEntityOnDestroy() {};
	virtual void DeadUpdate() {};
	virtual void LiveEntitiesOnCollisionHit([[maybe_unused]] ADXCollider* col, [[maybe_unused]] ADXCollider* myCol) {};
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol) final;
	void OnPreRender() final;

private:
	void UniqueInitialize() final;
	void UniqueUpdate() final;

public:
	static void StaticUpdate();
	static void SetAttackObj(const AttackObject& attackObj);
	static std::vector<AttackObject> GetAttackObj() { return S_attackObjs; };
};

struct AttackObject
{
	ADXCollider* col = nullptr;
	LiveEntity* attacker = nullptr;
	float power = 0;
};