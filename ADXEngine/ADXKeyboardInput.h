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

	//�S�L�[�̓��͏�Ԃ�����ϐ�
	BYTE key[256] = {};
	//�O�̃t���[���̓��͏�Ԃ�����ϐ�
	BYTE prevKey[256] = {};

public:
	ADXKeyBoardInput(ADXWindow* setWindow);
	void Update();
	bool KeyPress(const BYTE& keyNum);
	bool KeyTrigger(const BYTE& keyNum);
	bool KeyRelease(const BYTE& keyNum);
};