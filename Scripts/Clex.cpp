#include "Clex.h"

void Clex::EnemyUpdate()
{
	rigidbody->drag = 0.9f;
	rigidbody->dragAxis = { true,true,true };
	rigidbody->gravity = { 0,0,0 };

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

	if (attackProgress > 0.9f)
	{
		rigidbody->velocity = (GetGameObject()->transform.localPosition_ - cursor).Normalize();
	}
	attackProgress = min(max(0, attackProgress - 0.02f), 1);
}