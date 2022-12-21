#include "ADXWorldTransform.h"
#include <math.h>

ADXMatrix4* ADXWorldTransform::matView_ = nullptr;
ADXMatrix4* ADXWorldTransform::matProjection_ = nullptr;

void ADXWorldTransform::StaticInitialize(ADXMatrix4* matView, ADXMatrix4* matProjection)
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
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////回転//////
	//Z軸回転行列を宣言
	ADXMatrix4 matRotZ =
	{ (float)cos(rotation_.z),(float)sin(rotation_.z),0,0,
	(float)-sin(rotation_.z),(float)cos(rotation_.z),0,0,
	0,0,1,0,
	0,0,0,1 };
	//X軸回転行列を宣言
	ADXMatrix4 matRotX =
	{ 1,0,0,0,
	0,(float)cos(rotation_.x),(float)sin(rotation_.x),0,
	0,(float)-sin(rotation_.x),(float)cos(rotation_.x),0,
	0,0,0,1 };
	//Y軸回転行列を宣言
	ADXMatrix4 matRotY =
	{ (float)cos(rotation_.y),0,(float)-sin(rotation_.y),0,
	0,1,0,0,
	(float)sin(rotation_.y),0,(float)cos(rotation_.y),0,
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
	translation_.x,translation_.y,translation_.z,1 };


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

	//定数バッファに転送
	constMapTransform->matWorld = matWorld_;
	if (!rectTransform)
	{
		constMapTransform->matWorld *= *matView_;
		constMapTransform->matWorld *= *matProjection_;
	}
}