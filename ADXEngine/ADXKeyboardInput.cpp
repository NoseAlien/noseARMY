#include "ADXKeyBoardInput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

ADXKeyBoardInput* ADXKeyBoardInput::S_current = nullptr;

ADXKeyBoardInput::ADXKeyBoardInput(ADXWindow* setWindow)
{
	adxwindow = setWindow;

	result = DirectInput8Create(
		adxwindow->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		adxwindow->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	S_current = this;
}

void ADXKeyBoardInput::Update()
{
	//キーボード情報の取得開始
	keyboard->Acquire();

	//前のフレームの入力状態を取得する
	for (int32_t i = 0; i < sizeof(key) / sizeof(*key); i++)
	{
		prevKey[i] = key[i];
	}

	//全キーの入力状態を取得する
	keyboard->GetDeviceState(sizeof(key), key);

	S_current = this;
}

bool ADXKeyBoardInput::KeyPress(const BYTE& keyNum)
{
	return key[keyNum];
}

bool ADXKeyBoardInput::KeyTrigger(const BYTE& keyNum)
{
	return key[keyNum] && !prevKey[keyNum];
}

bool ADXKeyBoardInput::KeyRelease(const BYTE& keyNum)
{
	return !key[keyNum] && prevKey[keyNum];
}