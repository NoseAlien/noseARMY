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

	//�S�L�[�̓��͏�Ԃ�����ϐ�
	BYTE key_[256] = {};
	//�O�̃t���[���̓��͏�Ԃ�����ϐ�
	BYTE prevKey_[256] = {};

private:
	static ADXKeyBoardInput* S_current;

public:
	ADXKeyBoardInput(ADXWindow* setWindow);
	void Update();
	bool GetKey(const BYTE& keyNum);
	bool GetKeyDown(const BYTE& keyNum);
	bool GetKeyUp(const BYTE& keyNum);

public:
	static ADXKeyBoardInput* GetCurrentInstance() { return S_current; };
};