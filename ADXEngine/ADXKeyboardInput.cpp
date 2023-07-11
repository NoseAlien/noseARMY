#include "ADXKeyboardInput.h"

ADXKeyBoardInput::ADXKeyBoardInput(ADXWindow* setWindow)
{
	adxwindow = setWindow;

	result = DirectInput8Create(
		adxwindow->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	//���̓f�[�^�`���̃Z�b�g
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(result));

	//�r�����䃌�x���̃Z�b�g
	result = keyboard->SetCooperativeLevel(
		adxwindow->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(result));
}

void ADXKeyBoardInput::Update()
{
	//�L�[�{�[�h���̎擾�J�n
	keyboard->Acquire();

	//�O�̃t���[���̓��͏�Ԃ��擾����
	for (int i = 0; i < sizeof(key) / sizeof(*key); i++)
	{
		prevKey[i] = key[i];
	}

	//�S�L�[�̓��͏�Ԃ��擾����
	keyboard->GetDeviceState(sizeof(key), key);
}

bool ADXKeyBoardInput::KeyPress(BYTE keyNum)
{
	return key[keyNum];
}

bool ADXKeyBoardInput::KeyTrigger(BYTE keyNum)
{
	return key[keyNum] && !prevKey[keyNum];
}

bool ADXKeyBoardInput::KeyRelease(BYTE keyNum)
{
	return !key[keyNum] && prevKey[keyNum];
}