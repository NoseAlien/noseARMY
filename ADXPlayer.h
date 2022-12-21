#pragma once

#include "ADXComponent.h"
#include "ADXKeyBoardInput.h"
#include "ADXVector3.h"
#include "ADXAudio.h"
#include <vector>

class ADXObject;

class ADXPlayer : public ADXComponent
{
public:
	ADXPlayer(ADXObject* obj, ADXKeyBoardInput* setKeyboard, std::vector<int> setConfig);

private:
	void UniqueUpdate();

private:
	ADXAudio se{};
	ADXKeyBoardInput* keyboard = nullptr;
	std::vector<int> config{};
	bool prevJump = false;
	ADXVector3 prevPos{};
	ADXVector3 velocity = { 0,0,0 };
};