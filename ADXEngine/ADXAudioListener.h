#pragma once
#include "ADXComponent.h"

class ADXAudioListener : public ADXComponent
{
public:
	float radius_ = 10;

private:
	//更新
	void UniqueUpdate();

	//メモリ管理
	void SafetyPhase();

private:
	static ADXAudioListener* S_current;

public:
	//このクラスのインスタンスを取得
	static ADXAudioListener* GetCurrentInstance() { return S_current; }
};