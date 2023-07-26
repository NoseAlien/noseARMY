#include "ADXWorldTransform.h"
#include "ADXCamera.h"
#include <math.h>
#include <cmath>

ADXMatrix4* ADXWorldTransform::S_matView = nullptr;
ADXMatrix4* ADXWorldTransform::S_matProjection = nullptr;

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
	{ localScale.x,0,0,0,
	0,localScale.y,0,0,
	0,0,localScale.z,0,
	0,0,0,1 };
	ADXMatrix4 matRot = localRotation.RotateMatrix();
	ADXMatrix4 matTrans =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition.x,localPosition.y,localPosition.z,1 };

	ADXMatrix4 matWorld = IdentityMatrix();
	matWorld = matWorld * matScale * matRot * matTrans;

	return matWorld;
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

	//‡¬—p‰ñ“]s—ñ‚ðéŒ¾‚µAZXY‚Ì‡‚É‡¬
	matRot_ = localRotation_.RotateMatrix();
	/*ADXVector3 eul = ADXQuaternion::QuaternionToEuler(localRotation_);

	ADXMatrix4 matRotX = ADXMatrix4(
		1, 0, 0, 0,
		0, cosf(eul.x), -sinf(eul.x), 0,
		0, sinf(eul.x), cosf(eul.x), 0,
		0, 0, 0, 1
	);

	ADXMatrix4 matRotY = ADXMatrix4(
		cosf(eul.y), 0, sinf(eul.y), 0,
		0, 1, 0, 0,
		-sinf(eul.y), 0, cosf(eul.y), 0,
		0, 0, 0, 1
	);

	ADXMatrix4 matRotZ = ADXMatrix4(
		cosf(eul.z), -sinf(eul.z), 0, 0,
		sinf(eul.z), cosf(eul.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	matRot_ = matRotZ * matRotX;
	matRot_ *= matRotY;*/


	//////•½sˆÚ“®//////
	//ˆÚ“®—Ê‚ðs—ñ‚ÉÝ’è‚·‚é
	matTrans_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	localPosition_.x,localPosition_.y,localPosition_.z,1 };


	//////•½sˆÚ“®‹ts—ñ//////
	ADXMatrix4 matTransInv =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	-localPosition_.x,-localPosition_.y,-localPosition_.z,1 };


	//’PˆÊs—ñ‚ð‘ã“ü
	matWorld_ = IdentityMatrix();
	//ƒXƒP[ƒŠƒ“ƒOA‰ñ“]AˆÚ“®s—ñ‚ð‡‚ÉŠ|‚¯‚é
	matWorld_ = matWorld_ * matScale_ * matRot_ * matTrans_;

	//’PˆÊs—ñ‚ð‘ã“ü
	matWorldInverse_ = IdentityMatrix();
	//ˆÚ“®A‰ñ“]AƒXƒP[ƒŠƒ“ƒO‚Ì‹ts—ñ‚ð‡‚ÉŠ|‚¯‚é
	matWorldInverse_ = matWorldInverse_ * matTransInv * matRot_.Transpose() * matScale_.Inverse();

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;//e‚Ìs—ñ‚ðŠ|‚¯ŽZ‚·‚é
		matScale_ *= parent_->matScale_;//e‚ÌƒXƒP[ƒŠƒ“ƒOs—ñ‚àŠ|‚¯ŽZ‚·‚é
		matRot_ *= parent_->matRot_;//e‚Ì‰ñ“]s—ñ‚àŠ|‚¯ŽZ‚·‚é
		matTrans_ *= parent_->matTrans_;//e‚Ì•½sˆÚ“®s—ñ‚àŠ|‚¯ŽZ‚·‚é
		matWorldInverse_ = parent_->matWorldInverse_ * matWorldInverse_;//e‚Ì‹ts—ñ‚ðŠ|‚¯‚éi‡”Ô‚Í‹t‚Éj
	}
}

void ADXWorldTransform::UpdateConstBuffer()
{
	//’è”ƒoƒbƒtƒ@‚É“]‘—
	constMapTransform->matWorld = GenerateMatTransform(modelPosition_,modelRotation_,modelScale_) * matWorld_;
	constMapTransform->matWorldRot = modelRotation_.RotateMatrix() * matRot_;
	constMapTransform->matMVP = constMapTransform->matWorld;
	if (!rectTransform)
	{
		constMapTransform->matMVP = constMapTransform->matMVP * *S_matView * *S_matProjection;
	}
	constMapTransform->cameraWorldPos = ADXCamera::GetCameraWorldPos();
}

ADXVector3 ADXWorldTransform::GetWorldPosition()
{
	UpdateMatrix();
	return ADXMatrix4::transform({ 0,0,0 }, matWorld_);
}

void ADXWorldTransform::SetWorldPosition(const ADXVector3& worldPos)
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

ADXMatrix4 ADXWorldTransform::GetMatWorldInverse()
{
	ADXMatrix4 ret = matWorld_.Inverse();
	
	if (std::isnan(ret.m[0][0])
	|| std::isnan(ret.m[1][1])
	|| std::isnan(ret.m[2][2]))
	{
		ret = matWorldInverse_;
	}

	return ret;
}
