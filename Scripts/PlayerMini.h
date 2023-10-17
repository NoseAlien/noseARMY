#pragma once

#include "ADXObject.h"
#include "ADXRigidbody.h"

class Player;

class PlayerMini : public ADXComponent
{
private:
	ADXObject* nose = nullptr;
	Player* parent = nullptr;

	bool destroyFlag = false;
	ADXRigidbody* rigidbody = nullptr;

public:
	void Initialize(Player* setParent, const ADXObject& setNose);
	Player* GetParent() { return parent; };
		
private:
	void UniqueUpdate();
	void OnDestroy();
	void Move(float walkSpeed, float jumpPower);
};