#include "Clex.h"
#include "ADXCamera.h"
#include "ADXUtility.h"
#include "Projectile.h"

void Clex::EnemyInitialize()
{
	rect_ = ADXModel::CreateRect();
	enemyModel_ = ADXModel::LoadADXModel("model/Clex.obj");

	nutralTex_ = ADXImage::LoadADXImage("texture/tex_Clex.png");
	deadTex_ = ADXImage::LoadADXImage("texture/tex_Clex_3.png");
	attackTex = ADXImage::LoadADXImage("texture/tex_Clex_2.png");

	visual_->model_ = &enemyModel_;

	face = ADXObject::Create();
	face->transform_.parent_ = &visual_->transform_;
	face->transform_.localPosition_ = { 0,-0.4f,0.9f };
	face->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.4f,0,0 });
	face->model_ = &rect_;
	face->texture_ = ADXImage::LoadADXImage("texture/Clex_face.png");
	bodyParts_.push_back(face);

	body_ = ADXObject::Create();
	body_->transform_.parent_ = &visual_->transform_;
	body_->model_ = &rect_;
	body_->texture_ = ADXImage::LoadADXImage("texture/Clex_projectile.png");
	bodyParts_.push_back(body_);

	antennaRig = ADXObject::Create();
	antennaRig->transform_.parent_ = &visual_->transform_;

	antenna = ADXObject::Create();
	antenna->transform_.parent_ = &antennaRig->transform_;
	antenna->model_ = &rect_;
	antenna->texture_ = ADXImage::LoadADXImage("texture/Clex_antenna.png");
	bodyParts_.push_back(antenna);
}

void Clex::EnemyUpdate()
{
	rigidbody_->drag_ = 0.9f;
	rigidbody_->dragAxis_ = { true,true,true };
	rigidbody_->gravity_ = { 0,0,0 };

	GetGameObject()->sortingOrder_ = 1;

	bodyScale = ADXUtility::Lerp(bodyScale, 1, 0.4f);
	antennaAngle = ADXUtility::Lerp(antennaAngle, 0, 0.05f);

	if (targetDetected_ && attackProgress_ <= 0)
	{
		ADXVector3 targetRelativePos = targetPos_;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
		}
		cursor_ = targetRelativePos;
		attackProgress_ = 1;
	}

	GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress_ > 0)
	{
		visual_->texture_ = attackTex;

		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(cursor_.x_ - GetGameObject()->transform_.localPosition_.x_,cursor_.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

		GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, 0.3f);
		GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();
		if (attackProgress_ > 0.7f)
		{

		}
		else if (attackProgress_ > 0.6f)
		{
			rigidbody_->velocity_ = (GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * 0.3f;
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

				projectile->SetData((GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * -0.5f, ADXImage::LoadADXImage("texture/Clex_projectile.png"));
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
	antennaRig->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ antennaAngle,0,0 });

}

void Clex::LiveEntitiesOnPreRender()
{
	body_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());

	antennaRig->transform_.localPosition_ = ADXQuaternion::RotateVector({ 0,1,0 }, GetGameObject()->transform_.modelRotation_);

	antenna->transform_.localPosition_ = { 0,1,0 };
	antenna->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	ADXVector3 cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition(),
		antennaRig->transform_.GetMatWorldInverse());
	antenna->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,2,0 }, (float)atan2(cameraRelativePos.x_ - antenna->transform_.localPosition_.x_, cameraRelativePos.z_ - antenna->transform_.localPosition_.z_))
		* antenna->transform_.localRotation_;
}
