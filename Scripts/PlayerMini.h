#pragma once

#include "ADXObject.h"
#include "ADXRigidbody.h"

class Player;

class PlayerMini : public ADXComponent
{
private:
	ADXObject nose{};
	Player* parent = nullptr;

	bool destroyFlag = false;
	ADXRigidbody rigidbody{};

public:
	void Initialize(Player* setParent, ADXObject setNose);
	Player* GetParent() { return parent; };
		
private:
	void UniqueUpdate();
	void Move(float walkSpeed, float jumpPower);

private:
	static std::vector<PlayerMini*> S_minis;
	static std::vector<PlayerMini*> S_allMiniPtr;

public:
	static void StaticUpdate();
	static std::vector<PlayerMini*> GetMinis() { return S_minis; };
};