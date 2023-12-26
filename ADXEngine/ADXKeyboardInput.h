#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <cassert>
#include <memory>
#include <wrl.h>
#include "ADXWindow.h"

//キーボードの入力を受け付けるクラス
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

public:
	//コンストラクタ
	ADXKeyBoardInput(ADXWindow* setWindow);
	
	//更新処理
	void Update();

	//引数のキーが押されている時にtrueを返す
	bool GetKey(const BYTE& keyNum);

	//引数のキーが押された瞬間にtrueを返す
	bool GetKeyDown(const BYTE& keyNum);

	//引数のキーを離した瞬間にtrueを返す
	bool GetKeyUp(const BYTE& keyNum);

private:
	static ADXKeyBoardInput* S_current;

public:
	//このクラスのインスタンスを取得
	static ADXKeyBoardInput* GetCurrentInstance() { return S_current; };
};