#include "ADXGamePadInput.h"
#pragma comment (lib, "xinput.lib")

ADXGamePadInput* ADXGamePadInput::S_current = nullptr;

void ADXGamePadInput::Update()
{
	prevInputState_ = inputState_;
	ZeroMemory(&inputState_, sizeof(XINPUT_STATE));

	//コントローラー取得
	DWORD dwResult = XInputGetState(0, &inputState_);

	if (dwResult == ERROR_SUCCESS)
	{
		S_current = this;
	}
	else
	{
		//コントローラーが接続されていない
	}
}

bool ADXGamePadInput::GetButton(const ControllerButton& buttonNum)
{
	if (buttonNum == LT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < inputState_.Gamepad.bLeftTrigger;
	}
	else if (buttonNum == RT) {
		return XINPUT_GAMEPAD_TRIGGER_THRESHOLD < inputState_.Gamepad.bRightTrigger;
	}
	else {
		return inputState_.Gamepad.wButtons & buttonNum;
	}
}

bool ADXGamePadInput::GetButtonDown(const ControllerButton& buttonNum)
{
	if (buttonNum == LT) {
		return prevInputState_.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && GetButton(buttonNum);
	}
	else if (buttonNum == RT) {
		return prevInputState_.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD && GetButton(buttonNum);
	}
	else {
		return !(prevInputState_.Gamepad.wButtons & buttonNum) && GetButton(buttonNum);
	}
}

bool ADXGamePadInput::GetButtonUp(const ControllerButton& buttonNum)
{
	if (buttonNum == LT) {
		return !(XINPUT_GAMEPAD_TRIGGER_THRESHOLD < inputState_.Gamepad.bLeftTrigger);
	}
	else if (buttonNum == RT) {
		return !(XINPUT_GAMEPAD_TRIGGER_THRESHOLD < inputState_.Gamepad.bRightTrigger);
	}
	else {
		return !(inputState_.Gamepad.wButtons & buttonNum);
	}
}

ADXVector2 ADXGamePadInput::GetLeftStickVec()
{
	ADXVector2 ret(static_cast<float>(inputState_.Gamepad.sThumbLX), static_cast<float>(-inputState_.Gamepad.sThumbLY));
	return ret / MaxStickInput;
}

ADXVector2 ADXGamePadInput::GetRightStickVec()
{
	ADXVector2 ret(static_cast<float>(inputState_.Gamepad.sThumbRX), static_cast<float>(-inputState_.Gamepad.sThumbRY));
	return ret / MaxStickInput;
}
