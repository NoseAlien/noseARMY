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
	//////�g�k//////
	//�X�P�[�����O�{�����s��ɐݒ肷��
	matScale_ =
	{ localScale_.x,0,0,0,
	0,localScale_.y,0,0,
	0,0,localScale_.z,0,
	0,0,0,1 };

	//////��]//////
	//Z����]�s���錾
	ADXMatrix4 matRotZ =
	{ (float)cos(localEulerAngles_.z),(float)sin(localEulerAngles_.z),0,0,
	(float)-sin(localEulerAngles_.z),(float)cos(localEulerAngles_.z),0,0,
	0,0,1,0,
	0,0,0,1 };
	//X����]�s���錾
	ADXMatrix4 matRotX =
	{ 1,0,0,0,
	0,(float)cos(localEulerAngles_.x),(float)sin(localEulerAngles_.x),0,
	0,(float)-sin(localEulerAngles_.x),(float)cos(localEulerAngles_.x),0,
	0,0,0,1 };
	//Y����]�s���錾
	ADXMatrix4 matRotY =
	{ (float)cos(localEulerAngles_.y),0,(float)-sin(localEulerAngles_.y),0,
	0,1,0,0,
	(float)sin(localEulerAngles_.y),0,(float)cos(localEulerAngles_.y),0,
	0,0,0,1 };

	//�����p��]�s���錾���AZXY�̏��ɍ���
	matRot_ = matRotZ;
	matRot_ *= matRotX;
	matRot_ *= matRotY;

	//////���s�ړ�//////
	//�ړ��ʂ��s��ɐݒ肷��
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition_.x,localPosition_.y,localPosition_.z,1 };


	//�P�ʍs�����
	matWorld_ = IdentityMatrix();
	//�X�P�[�����O�s����|����
	matWorld_ *= matScale_;
	//�����p��]�s����|����
	matWorld_ *= matRot_;
	//���s�ړ��s����|����
	matWorld_ *= matTrans_;

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;//�e�̍s����|���Z����
		matScale_ *= parent_->matScale_;//�e�̃X�P�[�����O�s����|���Z����
		matRot_ *= parent_->matRot_;//�e�̉�]�s����|���Z����
		matTrans_ *= parent_->matTrans_;//�e�̕��s�ړ��s����|���Z����
	}
}

void ADXWorldTransform::UpdateConstBuffer()
{
	//�萔�o�b�t�@�ɓ]��
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
