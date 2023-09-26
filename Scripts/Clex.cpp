#include "Clex.h"

void Clex::EnemyUpdate()
{
	rigidbody.drag = 0.9f;
	rigidbody.dragAxis = { true,true,true };
	rigidbody.gravity = { 0,0,0 };

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

	if (attackProgress > 0.9f)
	{
		rigidbody.velocity = (transform.localPosition_ - cursor).Normalize();
	}
	attackProgress = min(max(0, attackProgress - 0.02f), 1);
}