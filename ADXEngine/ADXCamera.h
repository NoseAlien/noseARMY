#pragma once
#include "ADXObject.h"

class ADXCamera : public ADXComponent
{

private:
	//�ˉe�ϊ��s��
	ADXMatrix4 matProjection{};

	//�r���[�ϊ��s��
	ADXMatrix4 matView{};
	DirectX::XMFLOAT3 eye{};
	DirectX::XMFLOAT3 target{};
	DirectX::XMFLOAT3 up{};

private:
	static ADXCamera* S_current;
	static ADXVector3 S_cameraWorldPos;

public:
	void Initialize();
	void PrepareToRandering();

private:
	void UniqueUpdate();

public:
	static ADXVector3 GetCameraWorldPos() { return S_cameraWorldPos; };
	static ADXCamera* GetCurrentCamera() { return S_current; };
};