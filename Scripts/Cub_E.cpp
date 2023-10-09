#include "Cub_E.h"

void Cub_E::EnemyInitialize()
{
	enemyModel = ADXModel::LoadADXModel("model/Cub_E.obj");

	GetGameObject()->model = &enemyModel;
	GetGameObject()->texture = ADXImage::LoadADXImage("tex_Cub_E.png");
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

	GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress > 0)
	{
		ADXVector3 finalTarget = cursor;
		if (attackProgress > 0.5f)
		{
			finalTarget.y += 6;
			rigidbody.velocity = (finalTarget - GetGameObject()->transform.localPosition_) * 0.05f;
		}
		else if (attackProgress > 0.2f)
		{
			GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress,0.5f,0.2f,1,0),2) * 3.1415f * 6,
				0 });
			if (attackProgress > 0.25f)
			{
				LiveEntity::SetAttackObj({ GetGameObject()->GetComponent<ADXCollider>(),this,10});
			}
		}
	}
	attackProgress = min(max(0, attackProgress - 0.006f), 1);
}