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
	//////Šgk//////
	//ƒXƒP[ƒŠƒ“ƒO”{—¦‚ðs—ñ‚ÉÝ’è‚·‚é
	matScale_ =
	{ scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////‰ñ“]//////
	//ZŽ²‰ñ“]s—ñ‚ðéŒ¾
	ADXMatrix4 matRotZ =
	{ (float)cos(rotation_.z),(float)sin(rotation_.z),0,0,
	(float)-sin(rotation_.z),(float)cos(rotation_.z),0,0,
	0,0,1,0,
	0,0,0,1 };
	//XŽ²‰ñ“]s—ñ‚ðéŒ¾
	ADXMatrix4 matRotX =
	{ 1,0,0,0,
	0,(float)cos(rotation_.x),(float)sin(rotation_.x),0,
	0,(float)-sin(rotation_.x),(float)cos(rotation_.x),0,
	0,0,0,1 };
	//YŽ²‰ñ“]s—ñ‚ðéŒ¾
	ADXMatrix4 matRotY =
	{ (float)cos(rotation_.y),0,(float)-sin(rotation_.y),0,
	0,1,0,0,
	(float)sin(rotation_.y),0,(float)cos(rotation_.y),0,
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
	translation_.x,translation_.y,translation_.z,1 };


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

	//’è”ƒoƒbƒtƒ@‚É“]‘—
	constMapTransform->matWorld = matWorld_;
	if (!rectTransform)
	{
		constMapTransform->matWorld *= *matView_;
		constMapTransform->matWorld *= *matProjection_;
	}
}