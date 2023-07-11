#include "ADXRigidbody.h"

void ADXRigidbody::UniqueInitialize()
{
	prevPos = gameObject->transform.localPosition_;
}

void ADXRigidbody::VelocityMove()
{
	velocity = gameObject->transform.localPosition_ - prevPos;
	prevPos = gameObject->transform.localPosition_;

	if (dragAxis.x && dragAxis.y && dragAxis.z )
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
	gameObject->transform.localPosition_ += velocity;
}