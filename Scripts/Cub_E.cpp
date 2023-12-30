#include "Cub_E.h"
#include "ADXCamera.h"

const float attackProgressSpeed = 0.006f;
const float attackPower = 10;
const float gravity = -2;
const float jumpHeight = 6;
const float jumpRotEasePower = 6;
const float actKeyFrame_jump = 0.9f;
const float actKeyFrame_fall = 0.5f;
const float actKeyFrame_postAtk = 0.2f;
const float aimSpeed = 0.3f;
const float jumpSpeed = 0.05f;
const ADXVector3 hairPos = { 0,2,0 };
const ADXVector3 tailRigPos = { 0,-0.8f,-1 };
const ADXQuaternion tailRigRot = ADXQuaternion::EulerToQuaternion({ -1,0,0 });

void Cub_E::EnemyInitialize()
{
	rect_ = ADXModel::CreateRect();
	enemyModel_ = ADXModel::LoadADXModel("model/Cub_E.obj");

	nutralTex_ = ADXImage::LoadADXImage("texture/tex_Cub_E.png");
	deadTex_ = ADXImage::LoadADXImage("texture/tex_Cub_E_4.png");
	preAttackTex = ADXImage::LoadADXImage("texture/tex_Cub_E_2.png");
	attackTex = ADXImage::LoadADXImage("texture/tex_Cub_E_3.png");

	visual_->model_ = &enemyModel_;

	//髪
	hair = ADXObject::Create();
	hair->transform_.parent_ = &visual_->transform_;
	hair->model_ = &rect_;
	hair->texture_ = ADXImage::LoadADXImage("texture/Cub_E_hair.png");
	//体の一部として登録
	bodyParts_.push_back(hair);

	//尻尾のボーン
	tailRig = ADXObject::Create();
	tailRig->transform_.parent_ = &visual_->transform_;

	//尻尾
	tail = ADXObject::Create();
	tail->transform_.parent_ = &tailRig->transform_;
	tail->model_ = &rect_;
	tail->texture_ = ADXImage::LoadADXImage("texture/Cub_E_tail.png");
	//体の一部として登録
	bodyParts_.push_back(tail);
}

void Cub_E::EnemyUpdate()
{
	GetGameObject()->sortingOrder_ = 1;
	//攻撃動作中でない状態で自機を見つけたら
	if (targetDetected_ && attackProgress_ <= 0)
	{
		ADXVector3 targetRelativePos = targetPos_;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
		}

		//攻撃動作に移行
		cursor_ = targetRelativePos;
		attackProgress_ = 1;
	}

	visual_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	rigidbody_->gravity_ = { 0,gravity,0 };

	if (attackProgress_ > 0)
	{
		ADXVector3 finalTarget = cursor_;
		//自機の方を向く
		if (attackProgress_ > actKeyFrame_jump)
		{
			ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
				{ 0,(float)atan2(cursor_.x_ - GetGameObject()->transform_.localPosition_.x_,cursor_.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

			GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_,targetRot, aimSpeed);
			GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();

			visual_->texture_ = preAttackTex;
		}
		//飛び上がる
		else if (attackProgress_ > actKeyFrame_fall)
		{
			visual_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress_,actKeyFrame_jump,actKeyFrame_fall,1,0),jumpRotEasePower) * ADXUtility::Pi * -2,
				0,
				0 });

			finalTarget.y_ += jumpHeight;
			rigidbody_->velocity_ = (finalTarget - GetGameObject()->transform_.localPosition_) * jumpSpeed;
			visual_->texture_ = preAttackTex;

		}
		//落下して攻撃
		else if (attackProgress_ > actKeyFrame_postAtk)
		{
			for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
			{
				if (!itr->isTrigger_)
				{
					LiveEntity::SetAttackObj({ itr,this,attackPower });
				}
			}
			visual_->texture_ = attackTex;
		}
	}
	attackProgress_ = min(max(0, attackProgress_ - attackProgressSpeed), 1);

	tailRig->transform_.localPosition_ = tailRigPos;
	tailRig->transform_.localRotation_ = tailRigRot;
}

void Cub_E::LiveEntitiesOnPreRender()
{
	hair->transform_.localPosition_ = hairPos;
	hair->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	//髪をYビルボードにする
	ADXVector3 cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition(),
		GetGameObject()->transform_.GetMatWorldInverse());
	hair->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle({ 0,1,0 },
		(float)atan2(cameraRelativePos.x_ - hair->transform_.localPosition_.x_, cameraRelativePos.z_ - hair->transform_.localPosition_.z_))
		* hair->transform_.localRotation_;

	tail->transform_.localPosition_ = { 0,1,0 };
	tail->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	//尻尾をYビルボードにする
	cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition(),
		tailRig->transform_.GetMatWorldInverse());
	tail->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,1,0 },(float)atan2(cameraRelativePos.x_ - tail->transform_.localPosition_.x_, cameraRelativePos.z_ - tail->transform_.localPosition_.z_))
		* tail->transform_.localRotation_;
}
