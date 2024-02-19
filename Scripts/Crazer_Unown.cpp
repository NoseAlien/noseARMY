#include "Crazer_Unown.h"
#include "Projectile.h"

const int maxFootsNum = 4;
const float projectileSpeed = 0.5f;

void Crazer_Unown::EnemyInitialize()
{
}

void Crazer_Unown::EnemyUpdate()
{
	if (!foots_.empty())
	{
		ADXVector3 bodyWorldPoint{};
		for (auto& itr : foots_)
		{
			bodyWorldPoint += itr->GetGameObject()->transform_.GetWorldPosition();
		}

		GetGameObject()->GetComponent<ADXRigidbody>()->velocity_ =
			(bodyWorldPoint - GetGameObject()->transform_.GetWorldPosition()) * 0.5f;
	}

	switch (phase_)
	{
	case phase::walk:
		Walk();
		break;
	case phase::shot:
		Shot();
		break;
	default:
		Idol();
		break;
	}
}

void Crazer_Unown::EnemySafetyPhase()
{
	foots_.remove_if([=](auto& itr)
		{ return itr->GetGameObject()->GetDeleteFlag(); });
}

void Crazer_Unown::Idol()
{
	//自機を見つけたら
	if (targetDetected_)
	{
		ADXVector3 targetRelativePos = targetPos_;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
		}

		//歩行動作に移行
		cursor_ = targetRelativePos;
		attackProgress_ = 1;
		phase_ = phase::walk;
	}
}

void Crazer_Unown::Walk()
{
	if (attackProgress_ > 0)
	{

	}
	else
	{
		phase_ = phase::idol;
		//自機を見つけたら
		if (targetDetected_)
		{
			ADXVector3 targetRelativePos = targetPos_;
			if (GetGameObject()->transform_.parent_ != nullptr)
			{
				targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
			}

			//攻撃動作に移行
			cursor_ = targetRelativePos;
			attackProgress_ = 1;
			phase_ = phase::shot;
		}
	}
}

void Crazer_Unown::Shot()
{
	ADXObject* projectileObj = ADXObject::Create(
		GetGameObject()->transform_.GetWorldPosition(),
		GetGameObject()->transform_.GetWorldRotation(),
		{ 1,1,1 },
		GetGameObject()->transform_.parent_);
	Projectile* projectile = projectileObj->AddComponent<Projectile>();

	projectile->SetData((GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * -projectileSpeed, ADXImage::LoadADXImage("texture/Clex_projectile.png"));
	projectile->SetTeam(GetTeam());

	phase_ = phase::idol;
}