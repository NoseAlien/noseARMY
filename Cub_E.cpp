#include "Cub_E.h"

void Cub_E::EnemyUpdate()
{
	sortingOrder = 1;

	if (targetDetected && attackProgress <= 0)
	{
		ADXVector3 targetRelativePos = targetPos;
		if (transform.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos, transform.parent_->GetMatWorld());
		}
		cursor = targetRelativePos;
		attackProgress = 1;
	}

	transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });

	if (attackProgress > 0)
	{
		ADXVector3 finalTarget = cursor;
		if (attackProgress > 0.5f)
		{
			finalTarget.y += 6;
			rigidbody.velocity = (finalTarget - transform.localPosition_) * 0.05f;
		}
		else if (attackProgress > 0.2f)
		{
			transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,
				ADXUtility::EaseIn(ADXUtility::ValueMapping(attackProgress,0.5f,0.2f,1,0),2) * 3.1415f * 6,
				0 });
			if (attackProgress > 0.25f)
			{
				LiveEntity::SetAttackObj({ &colliders[0],this,10 });
			}
		}
	}
	attackProgress = min(max(0, attackProgress - 0.006f), 1);
}