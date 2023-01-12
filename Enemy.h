#pragma once

#include "ADXObject.h"

class Enemy : public ADXObject
{
protected:
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };

public:
	void Initialize();

private:
	void UniqueUpdate();
	void VelocityInitialize();
	void VelocityMove(float drag);
	void VelocityUpdate();
};
