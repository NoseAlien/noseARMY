#pragma once

#include "ADXObject.h"

class PlayerMini : public ADXObject
{
public:
	PlayerMini();

private:
	void UniqueUpdate();

private:
	bool destroyFlag = false;
};