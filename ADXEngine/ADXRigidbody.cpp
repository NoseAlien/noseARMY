﻿#include "ADXRigidbody.h"

void ADXRigidbody::UniqueInitialize()
{
	GetGameObject()->transform.UpdateMatrix();
	prevPos = GetGameObject()->transform.localPosition_;
	initializedThisFrame = true;
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
	if (!initializedThisFrame)
	{
		GetGameObject()->transform.localPosition_ += velocity;
	}
	initializedThisFrame = false;
}