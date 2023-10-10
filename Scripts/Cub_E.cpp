#include "Cub_E.h"

void Cub_E::EnemyInitialize()
{
	enemyModel = ADXModel::LoadADXModel("model/Cub_E.obj");

	nutralTex = ADXImage::LoadADXImage("tex_Cub_E.png");
	deadTex = ADXImage::LoadADXImage("tex_Cub_E_4.png");
	preAttackTex = ADXImage::LoadADXImage("tex_Cub_E_2.png");
	attackTex = ADXImage::LoadADXImage("tex_Cub_E_3.png");

	GetGameObject()->model = &enemyModel;
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

		GetGameObject()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(-(cursor.x - GetGameObject()->transform.localPosition_.x),-(cursor.z - GetGameObject()->transform.localPosition_.z)),0});
	}

	GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress > 0)
	{
		ADXVector3 finalTarget = cursor;
		if (attackProgress > 0.5f)
		{
			GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress,1,0.5f,1,0),6) * 3.1415f * 2,
				0,
				0 });

			finalTarget.y += 6;
			rigidbody->velocity = (finalTarget - GetGameObject()->transform.localPosition_) * 0.05f;
			GetGameObject()->texture = preAttackTex;

		}
		else if (attackProgress > 0.2f)
		{
			if (attackProgress > 0.25f)
			{
				for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
				{
					if (!itr->isTrigger)
					{
						LiveEntity::SetAttackObj({ itr,this,10 });
					}
				}
			}
			GetGameObject()->texture = attackTex;
		}
	}
	attackProgress = min(max(0, attackProgress - 0.006f), 1);
}