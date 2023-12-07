#pragma once
#include<dinput.h>
#include<DirectXMath.h>
#include<wrl.h>
#include <xinput.h>
#include "ADXVector2.h"

enum ControllerButton
{
	A = XINPUT_GAMEPAD_A,
	B = XINPUT_GAMEPAD_B,
	X = XINPUT_GAMEPAD_X,
	Y = XINPUT_GAMEPAD_Y,
	START = XINPUT_GAMEPAD_START,
	BACK = XINPUT_GAMEPAD_BACK,
	LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
	RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
	LT,
	RT
};

enum ControllerStick
{
	LEFT,
	RIGHT
};

class ADXGamePadInput
{
private:
	//最高入力強度
	const float MaxStickInput = 32768.0f;

private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> controller_ = nullptr;
	XINPUT_STATE inputState_{};
	XINPUT_STATE prevInputState_{};
	float deadZoneAmount_ = 0.5f;

private:
	static ADXGamePadInput* S_current;

public:
	void Update();
	bool GetButton(const ControllerButton& buttonNum);
	bool GetButtonDown(const ControllerButton& buttonNum);
	bool GetButtonUp(const ControllerButton& buttonNum);
	ADXVector2 GetStickVec(const ControllerStick& stickNum);

public:
	static ADXGamePadInput* GetCurrentInstance() { return S_current; };
};