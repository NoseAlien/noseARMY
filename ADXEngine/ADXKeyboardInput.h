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
	ADXWindow* window_ = nullptr;
	Microsoft::WRL::ComPtr<IDirectInput8> directInput_ = nullptr;
	IDirectInputDevice8* keyboard_ = nullptr;

	//全キーの入力状態を入れる変数
	BYTE key_[256] = {};
	//前のフレームの入力状態を入れる変数
	BYTE prevKey_[256] = {};

private:
	static ADXKeyBoardInput* S_current;

public:
	ADXKeyBoardInput(ADXWindow* setWindow);
	void Update();
	bool KeyPress(const BYTE& keyNum);
	bool KeyTrigger(const BYTE& keyNum);
	bool KeyRelease(const BYTE& keyNum);

public:
	static ADXKeyBoardInput* GetCurrentInstance() { return S_current; };
};