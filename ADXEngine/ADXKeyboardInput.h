#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include <memory>
#include <wrl.h>
#include "ADXWindow.h"

class ADXKeyBoardInput
{
private:
	ADXWindow* adxwindow = nullptr;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;
	HRESULT result = {};

	//全キーの入力状態を入れる変数
	BYTE key[256] = {};
	//前のフレームの入力状態を入れる変数
	BYTE prevKey[256] = {};

public:
	ADXKeyBoardInput(ADXWindow* setWindow);
	void Update();
	bool KeyPress(const BYTE& keyNum);
	bool KeyTrigger(const BYTE& keyNum);
	bool KeyRelease(const BYTE& keyNum);

private:
	static ADXKeyBoardInput* S_current;

public:
	static ADXKeyBoardInput* GetCurrentInstance() { return S_current; };
};