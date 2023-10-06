#include "ADXCamera.h"
#include "ADXWindow.h"

using namespace DirectX;

ADXVector3 ADXCamera::S_cameraWorldPos = {};
ADXCamera* ADXCamera::S_current = nullptr;

void ADXCamera::UniqueInitialize()
{
	//射影変換行列（透視投影）
	matProjection = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),//画角
			(float)ADXWindow::S_window_width / ADXWindow::S_window_height,//アスペクト比
			1.0f, 1000.0f));//ニア、ファークリップ

	//ビュー変換行列
	eye = XMFLOAT3(0, 0, -20);
	target = XMFLOAT3(0, 0, 0);
	up = XMFLOAT3(0, 1, 0);
}

void ADXCamera::UniqueUpdate()
{
	ADXObject::SetAllCameraPtr(this);
}

void ADXCamera::PrepareToRandering()
{
	GetGameObject()->transform.UpdateMatrix();

	eye = ADXMatrix4::Transform({ 0,0,0 }, GetGameObject()->transform.GetMatWorld()).ConvertToXMFloat3();
	target = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform.GetMatWorld()).ConvertToXMFloat3();
	up = ADXMatrix4::Transform({ 0,1,0 }, GetGameObject()->transform.GetMatRot()).ConvertToXMFloat3();

	matView = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));

	ADXWorldTransform::SetViewProjection(&matView, &matProjection);
	S_current = this;
}