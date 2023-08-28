#include "ADXCamera.h"
#include "ADXWindow.h"

using namespace DirectX;

ADXVector3 ADXCamera::S_cameraWorldPos = {};
ADXCamera* ADXCamera::S_current = nullptr;

void ADXCamera::Initialize()
{
	//�ˉe�ϊ��s��i�������e�j
	matProjection = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),//��p
			(float)ADXWindow::S_window_width / ADXWindow::S_window_height,//�A�X�y�N�g��
			1.0f, 1000.0f));//�j�A�A�t�@�[�N���b�v

	//�r���[�ϊ��s��
	eye = XMFLOAT3(0, 0, -20);
	target = XMFLOAT3(0, 0, 0);
	up = XMFLOAT3(0, 1, 0);
	matView = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));
}

void ADXCamera::UniqueUpdate()
{
	ADXObject::SetAllCameraPtr(this);
}

void ADXCamera::PrepareToRandering()
{
	transform.UpdateMatrix();

	eye = ADXMatrix4::Transform({ 0,0,0 }, transform.GetMatWorld()).ConvertToXMFloat3();
	target = ADXMatrix4::Transform({ 0,0,1 }, transform.GetMatWorld()).ConvertToXMFloat3();
	up = ADXMatrix4::Transform({ 0,1,0 }, transform.GetMatRot()).ConvertToXMFloat3();

	matView = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));

	ADXWorldTransform::SetViewProjection(&matView, &matProjection);
	S_current = this;
}