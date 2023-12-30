#include "Clex.h"
#include "ADXCamera.h"
#include "ADXUtility.h"
#include "Projectile.h"

const float attackProgressSpeed = 0.01f;
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
	attackTex = ADXImage::LoadADXImage("texture/tex_Clex_2.png");

	visual_->model_ = &enemyModel_;

	//顔
	face = ADXObject::Create();
	face->transform_.parent_ = &visual_->transform_;
	face->transform_.localPosition_ = facePos;
	face->transform_.localRotation_ = faceRot;
	face->model_ = &rect_;
	face->texture_ = ADXImage::LoadADXImage("texture/Clex_face.png");
	//体の一部として登録
	bodyParts_.push_back(face);

	//胴体
	body_ = ADXObject::Create();
	body_->transform_.parent_ = &visual_->transform_;
	body_->model_ = &rect_;
	body_->texture_ = ADXImage::LoadADXImage("texture/Clex_projectile.png");
	//体の一部として登録
	bodyParts_.push_back(body_);

	//ツノのボーン
	antennaRig = ADXObject::Create();
	antennaRig->transform_.parent_ = &visual_->transform_;

	//ツノ
	antenna = ADXObject::Create();
	antenna->transform_.parent_ = &antennaRig->transform_;
	antenna->model_ = &rect_;
	antenna->texture_ = ADXImage::LoadADXImage("texture/Clex_antenna.png");
	//体の一部として登録
	bodyParts_.push_back(antenna);
}

void Clex::EnemyUpdate()
{
	rigidbody_->drag_ = drag;
	rigidbody_->dragAxis_ = { true,true,true };
	rigidbody_->gravity_ = { 0,0,0 };

	GetGameObject()->sortingOrder_ = 1;

	bodyScale = ADXUtility::Lerp(bodyScale, 1, bodyScalingSpeed);
	antennaAngle = ADXUtility::Lerp(antennaAngle, 0, antennaRotSpeed);

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

	GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress_ > 0)
	{
		visual_->texture_ = attackTex;

		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(cursor_.x_ - GetGameObject()->transform_.localPosition_.x_,cursor_.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

		GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, aimSpeed);
		GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();
		if (attackProgress_ > actKeyFrame_shot)
		{

		}
		//弾を撃つ
		else if (attackProgress_ > actKeyFrame_postAtk)
		{
			rigidbody_->velocity_ = (GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * shotBackSpeed;
			bodyScale = 0;
			antennaAngle = 1;
			if (!shotted)
			{
				ADXObject* projectileObj = ADXObject::Create(
					GetGameObject()->transform_.GetWorldPosition(),
					GetGameObject()->transform_.GetWorldRotation(),
					{1,1,1},
					GetGameObject()->transform_.parent_);
				Projectile* projectile = projectileObj->AddComponent<Projectile>();

				projectile->SetData((GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * -projectileSpeed, ADXImage::LoadADXImage("texture/Clex_projectile.png"));
				projectile->LiveEntity::Initialize(GetTeam());

				projectiles.push_back(projectile);

				shotted = true;
			}
		}
		else
		{
			bodyScale = 0;
			shotted = false;
		}
	}
	attackProgress_ = min(max(0, attackProgress_ - 0.01f), 1);

	body_->transform_.localScale_ = { bodyScale,bodyScale,bodyScale };
	antennaRig->transform_.localPosition_ = ADXQuaternion::RotateVector({ 0,1,0 }, GetGameObject()->transform_.modelRotation_);
	antennaRig->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ antennaAngle,0,0 });

}

void Clex::LiveEntitiesOnPreRender()
{
	//胴体をビルボードにする
	body_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());

	antenna->transform_.localPosition_ = { 0,1,0 };
	antenna->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	//ツノをYビルボードにする
	ADXVector3 cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition(),
		antennaRig->transform_.GetMatWorldInverse());
	antenna->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,1,0 }, (float)atan2(cameraRelativePos.x_ - antenna->transform_.localPosition_.x_, cameraRelativePos.z_ - antenna->transform_.localPosition_.z_))
		* antenna->transform_.localRotation_;
}
