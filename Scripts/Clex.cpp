#include "Clex.h"
#include "ADXCamera.h"
#include "ADXUtility.h"
#include "Projectile.h"

const float actProgressSpeed = 0.01f;
const float drag = 0.9f;
const float actKeyFrame_shot = 0.6f;
const float actKeyFrame_postAtk = 0.5f;
const float aimSpeed = 0.3f;
const float shotBackSpeed = 0.3f;
const float projectileSpeed = 0.5f;
const float bodyScalingSpeed = 0.4f;
const float antennaRotSpeed = 0.05f;
const ADXVector3 facePos = { 0,-0.4f,0.9f };
const ADXQuaternion faceRot = ADXQuaternion::EulerToQuaternion({ 0.4f,0,0 });

void Clex::EnemyInitialize()
{
	rect_ = ADXModel::CreateRect();
	enemyModel_ = ADXModel::LoadADXModel("model/Clex.obj");

	nutralTex_ = ADXImage::LoadADXImage("texture/tex_Clex.png");
	deadTex_ = ADXImage::LoadADXImage("texture/tex_Clex_3.png");
	attackTex_ = ADXImage::LoadADXImage("texture/tex_Clex_2.png");

	visual_->GetComponent<ADXModelRenderer>()->model_ = &enemyModel_;

	//顔
	face_ = ADXObject::Create();
	face_->transform_.parent_ = &visual_->transform_;
	face_->transform_.localPosition_ = facePos;
	face_->transform_.localRotation_ = faceRot;
	face_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	face_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/Clex_face.png");
	//体の一部として登録
	bodyParts_.push_back(face_);

	//胴体
	body_ = ADXObject::Create();
	body_->transform_.parent_ = &visual_->transform_;
	body_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	body_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/Clex_projectile.png");
	//体の一部として登録
	bodyParts_.push_back(body_);

	//ツノのボーン
	antennaRig_ = ADXObject::Create();
	antennaRig_->transform_.parent_ = &visual_->transform_;

	//ツノ
	antenna_ = ADXObject::Create();
	antenna_->transform_.parent_ = &antennaRig_->transform_;
	antenna_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	antenna_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/Clex_antenna.png");
	//体の一部として登録
	bodyParts_.push_back(antenna_);
}

void Clex::EnemyUpdate()
{
	rigidbody_->drag_ = drag;
	rigidbody_->dragAxis_ = { true,true,true };
	rigidbody_->gravity_ = { 0,0,0 };

	GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	GetGameObject()->sortingOrder_ = 1;

	bodyScale_ = ADXUtility::Lerp(bodyScale_, 1, bodyScalingSpeed);
	antennaAngle_ = ADXUtility::Lerp(antennaAngle_, 0, antennaRotSpeed);

	switch (phase_)
	{
	case phase::attack:
		Attack();
		break;
	default:
		Idol();
		break;
	}

	actProgress_ = min(max(0, actProgress_ - 0.01f), 1);

	body_->transform_.localScale_ = { bodyScale_,bodyScale_,bodyScale_ };
	antennaRig_->transform_.localPosition_ = ADXQuaternion::RotateVector({ 0,1,0 }, GetGameObject()->transform_.modelRotation_);
	antennaRig_->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ antennaAngle_,0,0 });

}

void Clex::LiveEntitiesOnPreRender()
{
	//胴体をビルボードにする
	body_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());

	antenna_->transform_.localPosition_ = { 0,1,0 };
	antenna_->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	//ツノをYビルボードにする
	ADXVector3 cameraRelativePos = antennaRig_->transform_.InverseTransformPoint(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition());
	antenna_->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,1,0 }, (float)atan2(cameraRelativePos.x_ - antenna_->transform_.localPosition_.x_, cameraRelativePos.z_ - antenna_->transform_.localPosition_.z_))
		* antenna_->transform_.localRotation_;
}

void Clex::Idol()
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

void Clex::Attack()
{
	visual_->GetComponent<ADXModelRenderer>()->texture_ = attackTex_;

	ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
		{ 0,(float)atan2(cursor_.x_ - GetGameObject()->transform_.localPosition_.x_,cursor_.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

	GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, aimSpeed);
	GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();
	if (actProgress_ > actKeyFrame_shot)
	{

	}
	//弾を撃つ
	else if (actProgress_ > actKeyFrame_postAtk)
	{
		rigidbody_->velocity_ = (GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * shotBackSpeed;
		bodyScale_ = 0;
		antennaAngle_ = 1;
		if (!shotted_)
		{
			ADXObject* projectileObj = ADXObject::Create(
				GetGameObject()->transform_.GetWorldPosition(),
				GetGameObject()->transform_.GetWorldRotation(),
				{ 1,1,1 },
				GetGameObject()->transform_.parent_);
			Projectile* projectile = projectileObj->AddComponent<Projectile>();

			projectile->SetData((GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * -projectileSpeed, { ADXImage::LoadADXImage("texture/Clex_projectile.png") });
			projectile->SetTeam(GetTeam());

			shotted_ = true;
		}
	}
	else if (actProgress_ > 0)
	{
		bodyScale_ = 0;
		shotted_ = false;
	}
	else
	{
		phase_ = phase::idol;
	}
}