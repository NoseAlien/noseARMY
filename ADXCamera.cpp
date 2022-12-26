#include "ADXCamera.h"

const int* ADXCamera::window_width = nullptr;
const int* ADXCamera::window_height = nullptr;

void ADXCamera::StaticInitialize(const int* set_window_width, const int* set_window_height)
{
	window_width = set_window_width;
	window_height = set_window_height;
}

void ADXCamera::Initialize()
{
	//�ˉe�ϊ��s��i�������e�j
	matProjection = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),//��p
			(float)*window_width / *window_height,//�A�X�y�N�g��
			0.1f, 1000.0f));//�j�A�A�t�@�[�N���b�v

	//�r���[�ϊ��s��
	eye = XMFLOAT3(0, 0, -20);
	target = XMFLOAT3(0, 0, 0);
	up = XMFLOAT3(0, 1, 0);
	matView = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));

	ADXWorldTransform::StaticInitialize(&matView, &matProjection);
}

void ADXCamera::UniqueUpdate()
{

	matView = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));
}