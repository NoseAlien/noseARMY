#include "Cub_E.h"
#include "ADXCamera.h"

void Cub_E::EnemyInitialize()
{
	rect = ADXModel::CreateRect();
	enemyModel = ADXModel::LoadADXModel("model/Cub_E.obj");

	nutralTex = ADXImage::LoadADXImage("texture/tex_Cub_E.png");
	deadTex = ADXImage::LoadADXImage("texture/tex_Cub_E_4.png");
	preAttackTex = ADXImage::LoadADXImage("texture/tex_Cub_E_2.png");
	attackTex = ADXImage::LoadADXImage("texture/tex_Cub_E_3.png");

	visual->model = &enemyModel;

	hair = ADXObject::Create();
	hair->transform.parent_ = &visual->transform;
	hair->model = &rect;
	hair->texture = ADXImage::LoadADXImage("texture/Cub_E_hair.png");
	bodyParts.push_back(hair);

	tailRig = ADXObject::Create();
	tailRig->transform.parent_ = &visual->transform;

	tail = ADXObject::Create();
	tail->transform.parent_ = &tailRig->transform;
	tail->model = &rect;
	tail->texture = ADXImage::LoadADXImage("texture/Cub_E_tail.png");
	bodyParts.push_back(tail);
}

void Cub_E::EnemyUpdate()
{
	GetGameObject()->sortingOrder = 1;

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

	visual->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	rigidbody->gravity = { 0,-2,0 };

	if (attackProgress > 0)
	{
		ADXVector3 finalTarget = cursor;
		if (attackProgress > 0.9f)
		{
			ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
				{ 0,(float)atan2(cursor.x - GetGameObject()->transform.localPosition_.x,cursor.z - GetGameObject()->transform.localPosition_.z),0 });

			GetGameObject()->transform.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform.localRotation_,targetRot,0.3f);
			GetGameObject()->transform.localRotation_ = GetGameObject()->transform.localRotation_.Normalized();

			visual->texture = preAttackTex;
		}
		else if (attackProgress > 0.5f)
		{
			visual->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress,0.9f,0.5f,1,0),6) * 3.1415f * -2,
				0,
				0 });

			finalTarget.y += 6;
			rigidbody->velocity = (finalTarget - GetGameObject()->transform.localPosition_) * 0.05f;
			visual->texture = preAttackTex;

		}
		else if (attackProgress > 0.2f)
		{
			for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
			{
				if (!itr->isTrigger)
				{
					LiveEntity::SetAttackObj({ itr,this,10 });
				}
			}
			visual->texture = attackTex;
		}
	}
	attackProgress = min(max(0, attackProgress - 0.006f), 1);

	tailRig->transform.localPosition_ = { 0,-0.8f,-1 };
	tailRig->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ -1,0,0 });
}

void Cub_E::LiveEntitiesOnPreRender()
{
	hair->transform.localPosition_ = {0,2,0};
	hair->transform.localRotation_ = ADXQuaternion::IdentityQuaternion();

	ADXVector3 cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldPosition(),
		GetGameObject()->transform.GetMatWorldInverse());
	hair->transform.localRotation_ = ADXQuaternion::MakeAxisAngle({ 0,2,0 },
		(float)atan2(cameraRelativePos.x - hair->transform.localPosition_.x, cameraRelativePos.z - hair->transform.localPosition_.z))
		* hair->transform.localRotation_;

	tail->transform.localPosition_ = { 0,1,0 };
	tail->transform.localRotation_ = ADXQuaternion::IdentityQuaternion();

	cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldPosition(),
		tailRig->transform.GetMatWorldInverse());
	tail->transform.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,2,0 },(float)atan2(cameraRelativePos.x - tail->transform.localPosition_.x, cameraRelativePos.z - tail->transform.localPosition_.z))
		* tail->transform.localRotation_;
}
