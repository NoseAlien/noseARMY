#pragma once
#include<dinput.h>
#include<DirectXMath.h>
#include<wrl.h>
#include <xinput.h>
#include "ADXVector2.h"

//ゲームパッドの入力を受け付けるクラス
class ADXGamePadInput
{
public:
	enum controllerButton
	{
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
		LB = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RB = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		DPAD_UP = XINPUT_GAMEPAD_DPAD_UP,
		DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		LT,
		RT
	};

	enum controllerStick
	{
		STICK_LEFT,
		STICK_RIGHT
	};

private:
	//最高入力強度
	const float MaxStickInput = 32768.0f;

private:
	Microsoft::WRL::ComPtr<IDirectInputDevice8> controller_ = nullptr;
	XINPUT_STATE inputState_{};
	XINPUT_STATE prevInputState_{};
	float deadZoneAmount_ = 0.5f;

public:
	//更新処理
	void Update();

	//引数のボタンが押されている時にtrueを返す
	bool GetButton(const controllerButton& buttonNum);

	//引数のボタンが押された瞬間にtrueを返す
	bool GetButtonDown(const controllerButton& buttonNum);

	//引数のボタンを離した瞬間にtrueを返す
	bool GetButtonUp(const controllerButton& buttonNum);

	//引数の方向入力の値を返す
	ADXVector2 GetStickVec(const controllerStick& stickNum);

private:
	static ADXGamePadInput* S_current;

public:
	//このクラスのインスタンスを取得
	static ADXGamePadInput* GetCurrentInstance() { return S_current; };
};