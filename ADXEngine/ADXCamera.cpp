#include "ADXCamera.h"
#include "ADXWindow.h"

using namespace DirectX;

ADXVector3 ADXCamera::S_cameraWorldPos = {};
ADXCamera* ADXCamera::S_current = nullptr;

void ADXCamera::UniqueInitialize()
{
	//�ˉe�ϊ��s��i�������e�j
	matProjection_ = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),//��p
			(float)ADXWindow::S_window_width / ADXWindow::S_window_height,//�A�X�y�N�g��
			1.0f, 1000.0f));//�j�A�A�t�@�[�N���b�v

	//�r���[�ϊ��s��
	eye_ = XMFLOAT3(0, 0, -20);
	target_ = XMFLOAT3(0, 0, 0);
	up_ = XMFLOAT3(0, 1, 0);
}

void ADXCamera::UniqueUpdate()
{
	ADXObject::SetAllCameraPtr(this);
}

void ADXCamera::PrepareToRandering()
{
	GetGameObject()->transform_.UpdateMatrix();

	eye_ = ADXMatrix4::Transform({ 0,0,0 }, GetGameObject()->transform_.GetMatWorld()).ConvertToXMFloat3();
	target_ = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform_.GetMatWorld()).ConvertToXMFloat3();
	up_ = ADXMatrix4::Transform({ 0,1,0 }, GetGameObject()->transform_.GetMatRot()).ConvertToXMFloat3();

	matView_ = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_)));

	ADXWorldTransform::SetViewProjection(&matView_, &matProjection_);
	S_current = this;
}