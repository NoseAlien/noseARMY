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
	//////Šgk//////
	//ƒXƒP[ƒŠƒ“ƒO”{—¦‚ðs—ñ‚ÉÝ’è‚·‚é
	matScale_ =
	{ localScale_.x,0,0,0,
	0,localScale_.y,0,0,
	0,0,localScale_.z,0,
	0,0,0,1 };

	//////‰ñ“]//////
	//ZŽ²‰ñ“]s—ñ‚ðéŒ¾
	ADXMatrix4 matRotZ =
	{ (float)cos(localEulerAngles_.z),(float)sin(localEulerAngles_.z),0,0,
	(float)-sin(localEulerAngles_.z),(float)cos(localEulerAngles_.z),0,0,
	0,0,1,0,
	0,0,0,1 };
	//XŽ²‰ñ“]s—ñ‚ðéŒ¾
	ADXMatrix4 matRotX =
	{ 1,0,0,0,
	0,(float)cos(localEulerAngles_.x),(float)sin(localEulerAngles_.x),0,
	0,(float)-sin(localEulerAngles_.x),(float)cos(localEulerAngles_.x),0,
	0,0,0,1 };
	//YŽ²‰ñ“]s—ñ‚ðéŒ¾
	ADXMatrix4 matRotY =
	{ (float)cos(localEulerAngles_.y),0,(float)-sin(localEulerAngles_.y),0,
	0,1,0,0,
	(float)sin(localEulerAngles_.y),0,(float)cos(localEulerAngles_.y),0,
	0,0,0,1 };

	//‡¬—p‰ñ“]s—ñ‚ðéŒ¾‚µAZXY‚Ì‡‚É‡¬
	matRot_ = matRotZ;
	matRot_ *= matRotX;
	matRot_ *= matRotY;

	//////•½sˆÚ“®//////
	//ˆÚ“®—Ê‚ðs—ñ‚ÉÝ’è‚·‚é
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition_.x,localPosition_.y,localPosition_.z,1 };


	//’PˆÊs—ñ‚ð‘ã“ü
	matWorld_ = IdentityMatrix();
	//ƒXƒP[ƒŠƒ“ƒOs—ñ‚ðŠ|‚¯‚é
	matWorld_ *= matScale_;
	//‡¬—p‰ñ“]s—ñ‚ðŠ|‚¯‚é
	matWorld_ *= matRot_;
	//•½sˆÚ“®s—ñ‚ðŠ|‚¯‚é
	matWorld_ *= matTrans_;

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;//e‚Ìs—ñ‚ðŠ|‚¯ŽZ‚·‚é
		matScale_ *= parent_->matScale_;//e‚ÌƒXƒP[ƒŠƒ“ƒOs—ñ‚àŠ|‚¯ŽZ‚·‚é
		matRot_ *= parent_->matRot_;//e‚Ì‰ñ“]s—ñ‚àŠ|‚¯ŽZ‚·‚é
		matTrans_ *= parent_->matTrans_;//e‚Ì•½sˆÚ“®s—ñ‚àŠ|‚¯ŽZ‚·‚é
	}
}

void ADXWorldTransform::UpdateConstBuffer()
{
	//’è”ƒoƒbƒtƒ@‚É“]‘—
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
