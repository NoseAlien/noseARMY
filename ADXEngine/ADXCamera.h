#pragma once
#include "ADXObject.h"

class ADXCamera : public ADXComponent
{

private:
	//�ˉe�ϊ��s��
	ADXMatrix4 matProjection_{};

	//�r���[�ϊ��s��
	ADXMatrix4 matView_{};
	DirectX::XMFLOAT3 eye_{};
	DirectX::XMFLOAT3 target_{};
	DirectX::XMFLOAT3 up_{};

private:
	static ADXCamera* S_current;
	static ADXVector3 S_cameraWorldPos;

public:
	void PrepareToRandering();

private:
	void UniqueInitialize();
	void UniqueUpdate();

public:
	static ADXVector3 GetCameraWorldPos() { return S_cameraWorldPos; };
	static ADXCamera* GetCurrentCamera() { return S_current; };
};