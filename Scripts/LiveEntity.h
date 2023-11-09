#pragma once

#include "ADXObject.h"
#include "ADXAudio.h"
#include "ADXParticleSystem.h"
#include <string>

struct AttackObject;

class LiveEntity : public ADXComponent
{
protected:
	float maxHP = 999;
	float hpAmount = 1;
	ADXAudio damageSE{};
	ADXAudio defeatSE{};
	ADXObject* visual = nullptr;
	std::vector<ADXObject*> bodyParts{};

private:
	ADXModel rect{};
	uint32_t gaugeTex = 0;
	ADXParticleSystem* particle = nullptr;
	ADXObject* hpGauge = nullptr;
	ADXObject* hpGaugeBG = nullptr;
	std::string team = "";
	bool attackHitted = false;

public:
	void Initialize(const std::string& setTeam);
	bool IsLive() { return hpAmount > 0; };
	std::string GetTeam() { return team; };

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

private:
	static std::vector<AttackObject> S_attackObjs;
	static std::vector<AttackObject> S_allAttackObj;

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