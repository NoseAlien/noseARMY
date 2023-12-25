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
	//////�g�k//////
	matScale_ =
	{ localScale_.x_,0,0,0,
	0,localScale_.y_,0,0,
	0,0,localScale_.z_,0,
	0,0,0,1 };

	//////��]//////
	matRot_ = localRotation_.RotateMatrix();


	//////���s�ړ�//////
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition_.x_,localPosition_.y_,localPosition_.z_,1 };


	//////���s�ړ��t�s��//////
	ADXMatrix4 matTransInv =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	-localPosition_.x_,-localPosition_.y_,-localPosition_.z_,1 };


	//�P�ʍs�����
	matWorld_ = IdentityMatrix();
	//�X�P�[�����O�A��]�A�ړ��s������Ɋ|����
	matWorld_ = matWorld_ * matScale_ * matRot_ * matTrans_;

	//�P�ʍs�����
	matWorldInverse_ = IdentityMatrix();
	//�ړ��A��]�A�X�P�[�����O�̋t�s������Ɋ|����
	matWorldInverse_ = matWorldInverse_ * matTransInv * matRot_.Transpose() * matScale_.Inverse();

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;//�e�̍s����|���Z����
		matScale_ *= parent_->matScale_;//�e�̃X�P�[�����O�s����|���Z����
		matRot_ *= parent_->matRot_;//�e�̉�]�s����|���Z����
		matTrans_ *= parent_->matTrans_;//�e�̕��s�ړ��s����|���Z����
		matWorldInverse_ = parent_->matWorldInverse_ * matWorldInverse_;//�e�̋t�s����|����i���Ԃ͋t�Ɂj
	}
}

void ADXWorldTransform::UpdateConstBuffer()
{
	//�萔�o�b�t�@�ɓ]��
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

ADXMatrix4 ADXWorldTransform::GetMatWorldInverse()
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
