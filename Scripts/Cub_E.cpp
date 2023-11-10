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

	visual->model_ = &enemyModel;

	hair = ADXObject::Create();
	hair->transform_.parent_ = &visual->transform_;
	hair->model_ = &rect;
	hair->texture_ = ADXImage::LoadADXImage("texture/Cub_E_hair.png");
	bodyParts.push_back(hair);

	tailRig = ADXObject::Create();
	tailRig->transform_.parent_ = &visual->transform_;

	tail = ADXObject::Create();
	tail->transform_.parent_ = &tailRig->transform_;
	tail->model_ = &rect;
	tail->texture_ = ADXImage::LoadADXImage("texture/Cub_E_tail.png");
	bodyParts.push_back(tail);
}

void Cub_E::EnemyUpdate()
{
	GetGameObject()->sortingOrder_ = 1;

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

	visual->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	rigidbody->gravity_ = { 0,-2,0 };

	if (attackProgress > 0)
	{
		ADXVector3 finalTarget = cursor;
		if (attackProgress > 0.9f)
		{
			ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
				{ 0,(float)atan2(cursor.x_ - GetGameObject()->transform_.localPosition_.x_,cursor.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

			GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_,targetRot,0.3f);
			GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();

			visual->texture_ = preAttackTex;
		}
		else if (attackProgress > 0.5f)
		{
			visual->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress,0.9f,0.5f,1,0),6) * ADXUtility::Pi * -2,
				0,
				0 });

			finalTarget.y_ += 6;
			rigidbody->velocity_ = (finalTarget - GetGameObject()->transform_.localPosition_) * 0.05f;
			visual->texture_ = preAttackTex;

		}
		else if (attackProgress > 0.2f)
		{
			for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
			{
				if (!itr->isTrigger_)
				{
					LiveEntity::SetAttackObj({ itr,this,10 });
				}
			}
			visual->texture_ = attackTex;
		}
	}
	attackProgress = min(max(0, attackProgress - 0.006f), 1);

	tailRig->transform_.localPosition_ = { 0,-0.8f,-1 };
	tailRig->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ -1,0,0 });
}

void Cub_E::LiveEntitiesOnPreRender()
{
	hair->transform_.localPosition_ = {0,2,0};
	hair->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	ADXVector3 cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition(),
		GetGameObject()->transform_.GetMatWorldInverse());
	hair->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle({ 0,2,0 },
		(float)atan2(cameraRelativePos.x_ - hair->transform_.localPosition_.x_, cameraRelativePos.z_ - hair->transform_.localPosition_.z_))
		* hair->transform_.localRotation_;

	tail->transform_.localPosition_ = { 0,1,0 };
	tail->transform_.localRotation_ = ADXQuaternion::IdentityQuaternion();

	cameraRelativePos = ADXMatrix4::Transform(
		ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldPosition(),
		tailRig->transform_.GetMatWorldInverse());
	tail->transform_.localRotation_ = ADXQuaternion::MakeAxisAngle(
		{ 0,2,0 },(float)atan2(cameraRelativePos.x_ - tail->transform_.localPosition_.x_, cameraRelativePos.z_ - tail->transform_.localPosition_.z_))
		* tail->transform_.localRotation_;
}
