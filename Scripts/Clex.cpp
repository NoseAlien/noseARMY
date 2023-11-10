#include "Clex.h"
#include "ADXCamera.h"
#include "ADXUtility.h"
#include "Projectile.h"

void Clex::EnemyInitialize()
{
	rect = ADXModel::CreateRect();
	enemyModel = ADXModel::LoadADXModel("model/Clex.obj");

	nutralTex = ADXImage::LoadADXImage("texture/tex_Clex.png");
	deadTex = ADXImage::LoadADXImage("texture/tex_Clex_3.png");
	attackTex = ADXImage::LoadADXImage("texture/tex_Clex_2.png");

	visual->model_ = &enemyModel;

	face = ADXObject::Create();
	face->transform_.parent_ = &visual->transform_;
	face->transform_.localPosition_ = { 0,-0.4f,0.9f };
	face->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.4f,0,0 });
	face->model_ = &rect;
	face->texture_ = ADXImage::LoadADXImage("texture/Clex_face.png");
	bodyParts.push_back(face);

	body = ADXObject::Create();
	body->transform_.parent_ = &visual->transform_;
	body->model_ = &rect;
	body->texture_ = ADXImage::LoadADXImage("texture/Clex_projectile.png");
	bodyParts.push_back(body);

	antennaRig = ADXObject::Create();
	antennaRig->transform_.parent_ = &visual->transform_;

	antenna = ADXObject::Create();
	antenna->transform_.parent_ = &antennaRig->transform_;
	antenna->model_ = &rect;
	antenna->texture_ = ADXImage::LoadADXImage("texture/Clex_antenna.png");
	bodyParts.push_back(antenna);
}

void Clex::EnemyUpdate()
{
	rigidbody->drag_ = 0.9f;
	rigidbody->dragAxis_ = { true,true,true };
	rigidbody->gravity_ = { 0,0,0 };

	GetGameObject()->sortingOrder_ = 1;

	bodyScale = ADXUtility::Lerp(bodyScale, 1, 0.4f);
	antennaAngle = ADXUtility::Lerp(antennaAngle, 0, 0.05f);

	if (targetDetected && attackProgress <= 0)
	{
		ADXVector3 targetRelativePos = targetPos;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos, GetGameObject()->transform_.parent_->GetMatWorld());
		}
		cursor = targetRelativePos;
		attackProgress = 1;
	}

	GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress > 0)
	{
		visual->texture_ = attackTex;

		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(cursor.x_ - GetGameObject()->transform_.localPosition_.x_,cursor.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

		GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, 0.3f);
		GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();
		if (attackProgress > 0.7f)
		{

		}
		else if (attackProgress > 0.6f)
		{
			rigidbody->velocity_ = (GetGameObject()->transform_.localPosition_ - cursor).Normalize() * 0.5f;
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

				projectile->SetData((GetGameObject()->transform_.localPosition_ - cursor).Normalize() * -0.7f, ADXImage::LoadADXImage("texture/Clex_projectile.png"));
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
	attackProgress = min(max(0, attackProgress - 0.02f), 1);

	body->transform_.localScale_ = { bodyScale,bodyScale,bodyScale };
	antennaRig->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ antennaAngle,0,0 });

}

void Clex::LiveEntitiesOnPreRender()
{
	body->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());

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
