#include "ADXWorldTransform.h"
#include "ADXCamera.h"
#include "ADXObject.h"
#include <math.h>
#include <cmath>

ADXMatrix4* ADXWorldTransform::S_matView = nullptr;
ADXMatrix4* ADXWorldTransform::S_matProjection = nullptr;


std::list<ADXWorldTransform*> ADXWorldTransform::GetChilds()
{
	std::list<ADXWorldTransform*> ret = {};

	for (auto& itr : ADXObject::GetObjs())
	{
		if (itr->transform_.parent_ == this)
		{
			ret.push_back(&itr->transform_);
		}
	}

	return ret;
}

void ADXWorldTransform::SetViewProjection(ADXMatrix4* matView, ADXMatrix4* matProjection)
{
	S_matView = matView;
	S_matProjection = matProjection;
}

ADXMatrix4 ADXWorldTransform::GetViewProjection()
{
	return *S_matView * *S_matProjection;
}

ADXMatrix4 ADXWorldTransform::GenerateMatTransform(const ADXVector3& localPosition, const ADXQuaternion& localRotation, const ADXVector3& localScale)
{
	ADXMatrix4 matScale =
	{ localScale.x_,0,0,0,
	0,localScale.y_,0,0,
	0,0,localScale.z_,0,
	0,0,0,1 };
	ADXMatrix4 matRot = localRotation.RotateMatrix();
	ADXMatrix4 matTrans =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition.x_,localPosition.y_,localPosition.z_,1 };

	ADXMatrix4 matWorld = IdentityMatrix();
	matWorld = matWorld * matScale * matRot * matTrans;

	return matWorld;
}

void ADXWorldTransform::Initialize(ADXObject* obj)
{
	ADXObject::InitializeConstBufferTransform(constBuffTransform_, &constMapTransform_);

	matWorld_ = IdentityMatrix();
	gameObject_ = obj;
}

void ADXWorldTransform::UpdateMatrix()
{
	//////拡縮//////
	matScale_ =
	{ localScale_.x_,0,0,0,
	0,localScale_.y_,0,0,
	0,0,localScale_.z_,0,
	0,0,0,1 };

	//////回転//////
	matRot_ = localRotation_.RotateMatrix();


	//////平行移動//////
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition_.x_,localPosition_.y_,localPosition_.z_,1 };


	//////平行移動逆行列//////
	ADXMatrix4 matTransInv =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	-localPosition_.x_,-localPosition_.y_,-localPosition_.z_,1 };


	//単位行列を代入
	matWorld_ = IdentityMatrix();
	//スケーリング、回転、移動行列を順に掛ける
	matWorld_ = matWorld_ * matScale_ * matRot_ * matTrans_;

	//単位行列を代入
	matWorldInverse_ = IdentityMatrix();
	//移動、回転、スケーリングの逆行列を順に掛ける
	matWorldInverse_ = matWorldInverse_ * matTransInv * matRot_.Transpose() * matScale_.Inverse();

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;//親の行列を掛け算する
		matScale_ *= parent_->matScale_;//親のスケーリング行列も掛け算する
		matRot_ *= parent_->matRot_;//親の回転行列も掛け算する
		matTrans_ *= parent_->matTrans_;//親の平行移動行列も掛け算する
		matWorldInverse_ = parent_->matWorldInverse_ * matWorldInverse_;//親の逆行列を掛ける（順番は逆に）
	}
}

void ADXWorldTransform::UpdateConstBuffer()
{
	//定数バッファに転送
	constMapTransform_->matWorld = GenerateMatTransform(modelPosition_,modelRotation_,modelScale_) * matWorld_;
	constMapTransform_->matWorldRot = modelRotation_.RotateMatrix() * matRot_;
	constMapTransform_->matMVP = constMapTransform_->matWorld;
	if (!rectTransform_)
	{
		constMapTransform_->matMVP = constMapTransform_->matMVP * *S_matView * *S_matProjection;
	}
	constMapTransform_->cameraWorldPos = ADXCamera::GetCameraWorldPos();
}

ADXVector3 ADXWorldTransform::GetWorldPosition()
{
	UpdateMatrix();
	return ADXMatrix4::Transform({ 0,0,0 }, matWorld_);
}

void ADXWorldTransform::SetWorldPosition(const ADXVector3& worldPos)
{
	if (parent_ == nullptr)
	{
		localPosition_ = worldPos;
	}
	else
	{
		localPosition_ = ADXMatrix4::Transform(worldPos, parent_->matWorld_.Inverse());
	}
}

ADXQuaternion ADXWorldTransform::GetWorldRotation() const
{
	return TransformRotation(ADXQuaternion::IdentityQuaternion());
}

void ADXWorldTransform::SetWorldRotation(const ADXQuaternion& worldRot)
{
	if (parent_ == nullptr)
	{
		localRotation_ = worldRot;
	}
	else
	{
		localRotation_ = parent_->InverseTransformRotation(worldRot);
	}
}

ADXVector3 ADXWorldTransform::TransformPoint(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, GetMatWorld());
	return ret;
}

ADXVector3 ADXWorldTransform::InverseTransformPoint(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, GetMatWorldInverse());
	return ret;
}

ADXVector3 ADXWorldTransform::TransformPointWithoutTranslation(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, matScale_ * matRot_);
	return ret;
}

ADXVector3 ADXWorldTransform::InverseTransformPointWithoutTranslation(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, matRot_.Transpose() * matScale_.Inverse());
	return ret;
}

ADXVector3 ADXWorldTransform::TransformPointOnlyRotation(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, matRot_);
	return ret;
}

ADXVector3 ADXWorldTransform::InverseTransformPointOnlyRotation(const ADXVector3& pos) const
{
	ADXVector3 ret = ADXMatrix4::Transform(pos, matRot_.Transpose());
	return ret;
}

ADXQuaternion ADXWorldTransform::TransformRotation(const ADXQuaternion& rot) const
{
	ADXWorldTransform* parentPtr = parent_;
	ADXQuaternion quatWorld = localRotation_;
	while (true)
	{
		if (parentPtr == nullptr)
		{
			break;
		}
		else
		{
			quatWorld = quatWorld * parentPtr->localRotation_;
			parentPtr = parentPtr->parent_;
		}
	}
	ADXQuaternion ret = rot * quatWorld;
	return ret;
}

ADXQuaternion ADXWorldTransform::InverseTransformRotation(const ADXQuaternion& rot) const
{
	ADXWorldTransform* parentPtr = parent_;
	ADXQuaternion quatWorld = localRotation_.Inverse();
	while (true)
	{
		if (parentPtr == nullptr)
		{
			break;
		}
		else
		{
			quatWorld = parentPtr->localRotation_.Inverse() * quatWorld;
			parentPtr = parentPtr->parent_;
		}
	}
	ADXQuaternion ret = quatWorld * rot;
	return ret;
}

ADXMatrix4 ADXWorldTransform::GetMatWorldInverse() const
{
	ADXMatrix4 ret = matWorld_.Inverse();
	
	if (std::isnan(ret.m_[0][0])
	|| std::isnan(ret.m_[1][1])
	|| std::isnan(ret.m_[2][2]))
	{
		ret = matWorldInverse_;
	}

	return ret;
}
