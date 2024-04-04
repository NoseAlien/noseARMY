﻿#include "ADXCamera.h"
#include "ADXWindow.h"

using namespace DirectX;

ADXVector3 ADXCamera::S_cameraWorldPos = {};
ADXCamera* ADXCamera::S_current = nullptr;

void ADXCamera::UniqueInitialize()
{
	//射影変換行列（透視投影）
	matProjection_ = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),//画角
			(float)ADXWindow::GetInstance()->window_width_ / ADXWindow::GetInstance()->window_height_,//アスペクト比
			1.0f, 1000.0f));//ニア、ファークリップ

	//ビュー変換行列
	eye_ = XMFLOAT3(0, 0, -20);
	target_ = XMFLOAT3(0, 0, 0);
	up_ = XMFLOAT3(0, 1, 0);
}

void ADXCamera::UniqueUpdate()
{
	//このカメラのポインタをADXObjectに送る
	ADXObject::SetAllCameraPtr(this);
}

void ADXCamera::PrepareToRandering()
{
	//ビュー変換行列を生成するための情報をトランスフォームに合わせる
	eye_ = ADXMatrix4::Transform({ 0,0,0 }, GetGameObject()->transform_.GetMatWorld()).ConvertToXMFloat3();
	target_ = ADXMatrix4::Transform({ 0,0,1 }, GetGameObject()->transform_.GetMatWorld()).ConvertToXMFloat3();
	up_ = ADXMatrix4::Transform({ 0,1,0 }, GetGameObject()->transform_.GetMatRot()).ConvertToXMFloat3();

	//ビュー変換行列を生成、ADXWorldTransformに登録
	matView_ = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_)));
	ADXWorldTransform::SetViewProjection(&matView_, &matProjection_);

	//S_currentに自分を登録（これでGetCurrentCamera()で取得できる）
	S_current = this;
}