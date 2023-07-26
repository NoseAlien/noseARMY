#pragma once
#include "ADXObject.h"

class ADXCamera : public ADXObject
{
private:
	static ADXCamera* S_current;

private:
	//射影変換行列
	ADXMatrix4 matProjection{};

	//ビュー変換行列
	ADXMatrix4 matView{};
	DirectX::XMFLOAT3 eye{};
	DirectX::XMFLOAT3 target{};
	DirectX::XMFLOAT3 up{};

private:
	static ADXVector3 S_cameraWorldPos;

public:
	void Initialize();

private:
	void UniqueUpdate();

public:
	static ADXVector3 GetCameraWorldPos() { return S_cameraWorldPos; };
	static ADXCamera* GetCurrentCamera() { return S_current; };
};