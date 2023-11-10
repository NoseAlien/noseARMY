#pragma once

#include "ADXObject.h"
#include "ADXRigidbody.h"

class Player;

class PlayerMini : public ADXComponent
{
private:
	ADXObject* nose_ = nullptr;
	ADXObject* body_ = nullptr;
	ADXModel rect_{};

	Player* parent_ = nullptr;

	ADXRigidbody* rigidbody_ = nullptr;
	float bodyRotAngle_ = 0;
	bool destroyFlag_ = false;

public:
	void Initialize(Player* setParent);
	Player* GetParent() { return parent_; };
		
private:
	void UniqueInitialize() final;
	void UniqueUpdate() final;
	void OnPreRender() final;
	void Move(float walkSpeed, float jumpPower);
};