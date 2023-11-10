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
	float drag_ = 1;
	ADXVector3 gravity_ = { 0,-1,0 };
	float gravityScale_ = 0;
	bool3 dragAxis_ = { true,true,true };
	ADXVector3 velocity_ = { 0,0,0 };

private:
	ADXVector3 prevPos_{};
	bool initializedThisFrame_ = false;

public:
	void VelocityMove();
	ADXVector3 GetPrevPos() { return prevPos_; };

private:
	void UniqueInitialize();
	void UniqueUpdate();
};