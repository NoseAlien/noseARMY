#include "Cub_E.h"
#include "ADXCamera.h"

const float actProgressSpeed = 0.006f;
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
	preAttackTex_ = ADXImage::LoadADXImage("texture/tex_Cub_E_2.png");
	attackTex_ = ADXImage::LoadADXImage("texture/tex_Cub_E_3.png");

	jumpSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	jumpSE_->LoadADXAudio("sound/Cub_E_jump.wav");
	jumpSE_->useDistanceFade_ = true;

	//髪
	hair_ = ADXObject::Create();
	hair_->transform_.parent_ = &visual_->transform_;
	ADXModelRenderer* tempRenderer = hair_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/Cub_E_hair.png");
	//体の一部として登録
	bodyParts_.push_back(hair_);

	//尻尾のボーン
	tailRig_ = ADXObject::Create();
	tailRig_->transform_.parent_ = &visual_->transform_;

	//尻尾
	tail_ = ADXObject::Create();
	tail_->transform_.parent_ = &tailRig_->transform_;
	tempRenderer = tail_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = ADXImage::LoadADXImage("texture/Cub_E_tail.png");
	//体の一部として登録
	bodyParts_.push_back(tail_);
}

void Cub_E::EnemyUpdate()
{
	GetGameObject()->sortingOrder_ = 1;
	rigidbody_->gravity_ = { 0,gravity,0 };
	visual_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	switch (phase_)
	{
	case phase::attack:
		Attack();
		break;
	default:
		Idol();
		break;
	}

	prevActProgress_ = actProgress_;
	actProgress_ = min(max(0, actProgress_ - actProgressSpeed), 1);

	tailRig_->transform_.localPosition_ = tailRigPos;
	tailRig_->transform_.localRotation_ = tailRigRot;
}

void Cub_E::LiveEntitiesOnPreRender()
{
	hair_->transform_.localPosition_ = hairPos;
	hair_->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	//髪をYビルボードにする
	ADXVector3 cameraRelativePos = GetGameObject()->transform_.InverseTransformPoint(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition());
	hair_->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle({ 0,1,0 },
		(float)atan2(cameraRelativePos.x_ - hair_->transform_.localPosition_.x_, cameraRelativePos.z_ - hair_->transform_.localPosition_.z_))
		* hair_->transform_.localRotation_;

	tail_->transform_.localPosition_ = { 0,1,0 };
	tail_->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	//尻尾をYビルボードにする
	cameraRelativePos = tailRig_->transform_.InverseTransformPoint(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition());
	tail_->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,1,0 },(float)atan2(cameraRelativePos.x_ - tail_->transform_.localPosition_.x_, cameraRelativePos.z_ - tail_->transform_.localPosition_.z_))
		* tail_->transform_.localRotation_;
}

void Cub_E::Idol()
{
	//自機を見つけたら
	if (targetDetected_)
	{
		ADXVector3 targetRelativePos = targetPos_;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
		}

		//攻撃動作に移行
		cursor_ = targetRelativePos;
		actProgress_ = 1;
		phase_ = phase::attack;
	}
}

void Cub_E::Attack()
{
	//自機の方を向く
	if (actProgress_ > actKeyFrame_jump)
	{
		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(cursor_.x_ - GetGameObject()->transform_.localPosition_.x_,cursor_.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

		GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, aimSpeed);
		GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();

		visual_->GetComponent<ADXModelRenderer>()->texture_ = preAttackTex_;
	}
	//飛び上がる
	else if (actProgress_ > actKeyFrame_fall)
	{
		visual_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({
			ADXUtility::EaseIn(ADXUtility::ValueMapping(actProgress_,actKeyFrame_jump,actKeyFrame_fall,1,0),jumpRotEasePower) * ADXUtility::Pi * -2,
			0,
			0 });

		ADXVector3 finalTarget = cursor_ + ADXVector3{ 0,jumpHeight,0 };
		rigidbody_->velocity_ = (finalTarget - GetGameObject()->transform_.localPosition_) * jumpSpeed;
		visual_->GetComponent<ADXModelRenderer>()->texture_ = preAttackTex_;

		if (prevActProgress_ > actKeyFrame_jump)
		{
			jumpSE_->Play();
		}
	}
	//落下して攻撃
	else if (actProgress_ > actKeyFrame_postAtk)
	{
		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
			{
				LiveEntity::SetAttackObj({ itr,this,attackPower });
			}
		}
		visual_->GetComponent<ADXModelRenderer>()->texture_ = attackTex_;
	}
	else if (actProgress_ <= 0)
	{
		phase_ = phase::idol;
	}
}