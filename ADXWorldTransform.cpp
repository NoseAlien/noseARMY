#include "ADXWorldTransform.h"
#include <math.h>

ADXMatrix4* ADXWorldTransform::matView_ = nullptr;
ADXMatrix4* ADXWorldTransform::matProjection_ = nullptr;

void ADXWorldTransform::SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection)
{
	matView_ = matView;
	matProjection_ = matProjection;
}

void ADXWorldTransform::Initialize()
{
	matWorld_ = IdentityMatrix();
}

void ADXWorldTransform::UpdateMatrix()
{
	//////拡縮//////
	//スケーリング倍率を行列に設定する
	matScale_ =
	{ localScale_.x,0,0,0,
	0,localScale_.y,0,0,
	0,0,localScale_.z,0,
	0,0,0,1 };

	//////回転//////
	//Z軸回転行列を宣言
	ADXMatrix4 matRotZ =
	{ (float)cos(localEulerAngles_.z),(float)sin(localEulerAngles_.z),0,0,
	(float)-sin(localEulerAngles_.z),(float)cos(localEulerAngles_.z),0,0,
	0,0,1,0,
	0,0,0,1 };
	//X軸回転行列を宣言
	ADXMatrix4 matRotX =
	{ 1,0,0,0,
	0,(float)cos(localEulerAngles_.x),(float)sin(localEulerAngles_.x),0,
	0,(float)-sin(localEulerAngles_.x),(float)cos(localEulerAngles_.x),0,
	0,0,0,1 };
	//Y軸回転行列を宣言
	ADXMatrix4 matRotY =
	{ (float)cos(localEulerAngles_.y),0,(float)-sin(localEulerAngles_.y),0,
	0,1,0,0,
	(float)sin(localEulerAngles_.y),0,(float)cos(localEulerAngles_.y),0,
	0,0,0,1 };

	//合成用回転行列を宣言し、ZXYの順に合成
	matRot_ = matRotZ;
	matRot_ *= matRotX;
	matRot_ *= matRotY;

	//////平行移動//////
	//移動量を行列に設定する
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition_.x,localPosition_.y,localPosition_.z,1 };


	//単位行列を代入
	matWorld_ = IdentityMatrix();
	//スケーリング行列を掛ける
	matWorld_ *= matScale_;
	//合成用回転行列を掛ける
	matWorld_ *= matRot_;
	//平行移動行列を掛ける
	matWorld_ *= matTrans_;

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;//親の行列を掛け算する
		matScale_ *= parent_->matScale_;//親のスケーリング行列も掛け算する
		matRot_ *= parent_->matRot_;//親の回転行列も掛け算する
		matTrans_ *= parent_->matTrans_;//親の平行移動行列も掛け算する
	}
}

void ADXWorldTransform::UpdateConstBuffer()
{
	//定数バッファに転送
	constMapTransform->matWorld = matWorld_;
	if (!rectTransform)
	{
		constMapTransform->matWorld *= *matView_;
		constMapTransform->matWorld *= *matProjection_;
	}
}

ADXVector3 ADXWorldTransform::GetWorldPosition()
{
	UpdateMatrix();
	return ADXMatrix4::transform({0,0,0}, matWorld_);
}

void ADXWorldTransform::SetWorldPosition(ADXVector3 worldPos)
{
	if (parent_ == nullptr)
	{
		localPosition_ = worldPos;
	}
	else
	{
		localPosition_ = ADXMatrix4::transform(worldPos, parent_->matWorld_.Inverse());
	}
}
