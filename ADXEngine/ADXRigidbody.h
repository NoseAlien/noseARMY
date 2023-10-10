#pragma once

#include "ADXComponent.h"
#include "ADXVector3.h"
#include "ADXObject.h"

struct bool3
{
	bool x;
	bool y;
	bool z;
};

class ADXRigidbody : public ADXComponent
{
public:
	float drag = 1;
	ADXVector3 gravity = { 0,-1,0 };
	float gravityScale = 0;
	bool3 dragAxis = { true,true,true };
	ADXVector3 velocity = { 0,0,0 };

private:
	ADXVector3 prevPos{};
	bool initializedThisFrame = false;

public:
	void VelocityMove();
	ADXVector3 GetPrevPos() { return prevPos; };

private:
	void UniqueInitialize();
	void UniqueUpdate();
};