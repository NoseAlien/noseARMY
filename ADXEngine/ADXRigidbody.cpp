#include "ADXRigidbody.h"

void ADXRigidbody::UniqueInitialize()
{
	GetGameObject()->transform_.UpdateMatrix();
	prevPos_ = GetGameObject()->transform_.localPosition_;
	//先に初期化処理が呼ばれたことが分かるようにする
	initializedThisFrame_ = true;
}

void ADXRigidbody::VelocityMove()
{
	velocity_ = GetGameObject()->transform_.localPosition_ - prevPos_;
	prevPos_ = GetGameObject()->transform_.localPosition_;

	//空気抵抗を掛ける
	if (dragAxis_.x && dragAxis_.y && dragAxis_.z)
	{
		velocity_ *= drag_;
	}
	else
	{
		//特定の軸にのみ空気抵抗を掛ける場合はこっちを通る
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

	//重力
	velocity_ += gravity_ * gravityScale_;
}

void ADXRigidbody::UniqueUpdate()
{
	//初期化したフレームでなければリジッドボディの処理を行う
	if (!initializedThisFrame_)
	{
		GetGameObject()->transform_.localPosition_ += velocity_;
	}
	initializedThisFrame_ = false;
}