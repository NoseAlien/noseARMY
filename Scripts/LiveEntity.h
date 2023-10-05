#pragma once

#include "ADXObject.h"
#include "ADXParticleSystem.h"
#include <string>

struct AttackObject;

class LiveEntity : public ADXComponent
{
protected:
	float maxHP = 999;
	float hpAmount = 1;

private:
	ADXModel rect{};
	uint32_t gaugeTex = 0;
	ADXParticleSystem particle{};
	ADXObject* hpGauge{};
	ADXObject* hpGaugeBG{};
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
	virtual void DeadUpdate() {};
	virtual void LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol) {};
	void OnCollisionHit(ADXCollider* col, ADXCollider* myCol);
	void OnPreRender();

private:
	void UniqueInitialize() final;
	void UniqueUpdate() final;

private:
	static std::vector<LiveEntity*> S_liveEntities;
	static std::vector<LiveEntity*> S_allLiveEntitiesPtr;
	static std::vector<AttackObject> S_attackObjs;
	static std::vector<AttackObject> S_allAttackObj;

public:
	static void StaticUpdate();
	static void SetAttackObj(const AttackObject& attackObj);
	static std::vector<AttackObject> GetAttackObj() { return S_attackObjs; };
	static std::vector<LiveEntity*> GetLiveEntities() { return S_liveEntities; };
};

struct AttackObject
{
	ADXCollider* col = nullptr;
	LiveEntity* attacker = nullptr;
	float power = 0;
};