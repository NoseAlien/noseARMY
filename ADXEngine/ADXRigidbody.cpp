#include "ADXRigidbody.h"

void ADXRigidbody::UniqueInitialize()
{
	GetGameObject()->transform_.UpdateMatrix();
	prevPos_ = GetGameObject()->transform_.localPosition_;
	initializedThisFrame_ = true;
}

void ADXRigidbody::VelocityMove()
{
	velocity_ = GetGameObject()->transform_.localPosition_ - prevPos_;
	prevPos_ = GetGameObject()->transform_.localPosition_;

	if (dragAxis_.x && dragAxis_.y && dragAxis_.z)
	{
		velocity_ *= drag_;
	}
	else
	{
		if (dragAxis_.x)
		{
			velocity_.x_ *= drag_;
		}
		if (dragAxis_.y)
		{
			velocity_.y_ *= drag_;
		}
		if (dragAxis_.z)
		{
			velocity_.z_ *= drag_;
		}
	}

	velocity_ += gravity_ * gravityScale_;
}

void ADXRigidbody::UniqueUpdate()
{
	if (!initializedThisFrame_)
	{
		GetGameObject()->transform_.localPosition_ += velocity_;
	}
	initializedThisFrame_ = false;
}