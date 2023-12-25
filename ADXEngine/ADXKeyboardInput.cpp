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

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard_->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard_->SetCooperativeLevel(
		window_->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));

	S_current = this;
}

void ADXKeyBoardInput::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard_->Acquire();

	//�O�̃t���[���̓��͏�Ԃ��擾����
	for (int32_t i = 0; i < sizeof(key_) / sizeof(*key_); i++)
	{
		prevKey_[i] = key_[i];
	}

	//�S�L�[�̓��͏�Ԃ��擾����
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