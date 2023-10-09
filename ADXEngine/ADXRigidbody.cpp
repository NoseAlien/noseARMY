#include "ADXRigidbody.h"

void ADXRigidbody::UniqueInitialize()
{
	prevPos = GetGameObject()->transform.localPosition_;
	GetGameObject()->transform.UpdateMatrix();
}

void ADXRigidbody::VelocityMove()
{
	velocity = GetGameObject()->transform.localPosition_ - prevPos;
	prevPos = GetGameObject()->transform.localPosition_;

	if (dragAxis.x && dragAxis.y && dragAxis.z)
	{
		velocity *= drag;
	}
	else
	{
		if (dragAxis.x)
		{
			velocity.x *= drag;
		}
		if (dragAxis.y)
		{
			velocity.y *= drag;
		}
		if (dragAxis.z)
		{
			velocity.z *= drag;
		}
	}

	velocity += gravity * gravityScale;
}

void ADXRigidbody::UniqueUpdate()
{
	GetGameObject()->transform.localPosition_ += velocity;
}