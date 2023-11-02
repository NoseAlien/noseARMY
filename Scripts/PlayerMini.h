#pragma once

#include "ADXObject.h"
#include "ADXRigidbody.h"

class Player;

class PlayerMini : public ADXComponent
{
private:
	ADXObject* nose = nullptr;
	ADXObject* body = nullptr;
	ADXModel rect{};

	Player* parent = nullptr;

	ADXRigidbody* rigidbody = nullptr;
	float bodyRotAngle = 0;
	bool destroyFlag = false;

public:
	void Initialize(Player* setParent);
	Player* GetParent() { return parent; };
		
private:
	void UniqueInitialize() final;
	void UniqueUpdate() final;
	void OnPreRender() final;
	void Move(float walkSpeed, float jumpPower);
};