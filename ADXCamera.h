#pragma once
#include "ADXObject.h"

class ADXCamera : public ADXObject
{
private:
	static const int* window_width;
	static const int* window_height;

private:
	//�ˉe�ϊ��s��
	ADXMatrix4 matProjection{};

	//�r���[�ϊ��s��
	ADXMatrix4 matView{};
	XMFLOAT3 eye{};
	XMFLOAT3 target{};
	XMFLOAT3 up{};

public:
	void Initialize();

private:
	void UniqueUpdate();

public:
	static void StaticInitialize(const int* set_window_width, const int* set_window_height);
};