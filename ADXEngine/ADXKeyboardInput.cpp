#include "ADXKeyBoardInput.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

ADXKeyBoardInput* ADXKeyBoardInput::S_current = nullptr;

ADXKeyBoardInput::ADXKeyBoardInput(ADXWindow* setWindow)
{
	window_ = setWindow;

	HRESULT result;

	result = DirectInput8Create(
		window_->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput_, nullptr);
	assert(SUCCEEDED(result));

	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboard_, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard_->SetCooperativeLevel(
		window_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	S_current = this;
}

void ADXKeyBoardInput::Update()
{
	//キーボード情報の取得開始
	keyboard_->Acquire();

	//前のフレームの入力状態を取得する
	for (int32_t i = 0; i < sizeof(key_) / sizeof(*key_); i++)
	{
		prevKey_[i] = key_[i];
	}

	//全キーの入力状態を取得する
	keyboard_->GetDeviceState(sizeof(key_), key_);

	S_current = this;
}

bool ADXKeyBoardInput::GetKey(const BYTE& keyNum)
{
	return key_[keyNum];
}

bool ADXKeyBoardInput::GetKeyDown(const BYTE& keyNum)
{
	return key_[keyNum] && !prevKey_[keyNum];
}

bool ADXKeyBoardInput::GetKeyUp(const BYTE& keyNum)
{
	return !key_[keyNum] && prevKey_[keyNum];
}