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

	visual->model = &enemyModel;

	face = ADXObject::Create();
	face->transform.parent_ = &visual->transform;
	face->transform.localPosition_ = { 0,-0.4f,0.9f };
	face->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.4f,0,0 });
	face->model = &rect;
	face->texture = ADXImage::LoadADXImage("texture/Clex_face.png");
	bodyParts.push_back(face);

	body = ADXObject::Create();
	body->transform.parent_ = &visual->transform;
	body->model = &rect;
	body->texture = ADXImage::LoadADXImage("texture/Clex_projectile.png");
	bodyParts.push_back(body);

	antennaRig = ADXObject::Create();
	antennaRig->transform.parent_ = &visual->transform;

	antenna = ADXObject::Create();
	antenna->transform.parent_ = &antennaRig->transform;
	antenna->model = &rect;
	antenna->texture = ADXImage::LoadADXImage("texture/Clex_antenna.png");
	bodyParts.push_back(antenna);
}

void Clex::EnemyUpdate()
{
	rigidbody->drag = 0.9f;
	rigidbody->dragAxis = { true,true,true };
	rigidbody->gravity = { 0,0,0 };

	GetGameObject()->sortingOrder = 1;

	bodyScale = ADXUtility::Lerp(bodyScale, 1, 0.4f);
	antennaAngle = ADXUtility::Lerp(antennaAngle, 0, 0.05f);

	if (targetDetected && attackProgress <= 0)
	{
		ADXVector3 targetRelativePos = targetPos;
		if (GetGameObject()->transform.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos, GetGameObject()->transform.parent_->GetMatWorld());
		}
		cursor = targetRelativePos;
		attackProgress = 1;
	}

	GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress > 0)
	{
		visual->texture = attackTex;

		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(cursor.x - GetGameObject()->transform.localPosition_.x,cursor.z - GetGameObject()->transform.localPosition_.z),0 });

		GetGameObject()->transform.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform.localRotation_, targetRot, 0.3f);
		GetGameObject()->transform.localRotation_ = GetGameObject()->transform.localRotation_.Normalized();
		if (attackProgress > 0.7f)
		{

		}
		else if (attackProgress > 0.6f)
		{
			rigidbody->velocity = (GetGameObject()->transform.localPosition_ - cursor).Normalize() * 0.5f;
			bodyScale = 0;
			antennaAngle = 1;
			if (!shotted)
			{
				ADXObject* projectileObj = ADXObject::Create(
					GetGameObject()->transform.GetWorldPosition(),
					GetGameObject()->transform.GetWorldRotation(),
					{1,1,1},
					GetGameObject()->transform.parent_);
				Projectile* projectile = projectileObj->AddComponent<Projectile>();

				projectile->SetData((GetGameObject()->transform.localPosition_ - cursor).Normalize() * -0.7f, ADXImage::LoadADXImage("texture/Clex_projectile.png"));
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

	body->transform.localScale_ = { bodyScale,bodyScale,bodyScale };
	antennaRig->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ antennaAngle,0,0 });

}

void Clex::LiveEntitiesOnPreRender()
{
	body->transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldRotation());

	antennaRig->transform.localPosition_ = ADXQuaternion::RotateVector({ 0,1,0 }, GetGameObject()->transform.modelRotation_);

	antenna->transform.localPosition_ = { 0,1,0 };
	antenna->transform.localRotation_ = ADXQuaternion::IdentityQuaternion();

	ADXVector3 cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldPosition(),
		antennaRig->transform.GetMatWorldInverse());
	antenna->transform.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,2,0 }, (float)atan2(cameraRelativePos.x - antenna->transform.localPosition_.x, cameraRelativePos.z - antenna->transform.localPosition_.z))
		* antenna->transform.localRotation_;
}
