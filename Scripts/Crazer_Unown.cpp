#include "Crazer_Unown.h"
#include "Projectile.h"

const float actProgressSpeed = 0.006f;
const int maxFootsNum = 4;
const float projectileSpeed = 2;
const int maxShotInterval = 4;
const int projectileLifeTime = 90;
const float aimSpeed = 0.3f;
const float actKeyFrame_shot = 0.8f;
const float actKeyFrame_postAtk = 0.2f;
const int walkStepNum = 4;

void Crazer_Unown::EnemyInitialize()
{
}

void Crazer_Unown::EnemyUpdate()
{
	scale_ = 30;

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

	actProgress_ = min(max(0, actProgress_ - actProgressSpeed), 1);
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
		//狙う
		ADXVector3 targetRelativePos = targetPos_;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
		}

		//歩行動作に移行
		cursor_ = targetRelativePos;
		actProgress_ = 1;
		phase_ = phase::walk;
	}
}

void Crazer_Unown::Walk()
{
	if (actProgress_ > 0)
	{

	}
	else
	{
		//仮で待機フェーズへ移行する
		phase_ = phase::idol;
		//自機を見つけたら
		if (targetDetected_)
		{
			//狙う
			ADXVector3 targetRelativePos = targetPos_;
			if (GetGameObject()->transform_.parent_ != nullptr)
			{
				targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
			}

			//攻撃動作に移行
			cursor_ = targetRelativePos;
			actProgress_ = 1;
			phase_ = phase::shot;
		}
	}
}

void Crazer_Unown::Shot()
{
	//自機の方を向く
	if (actProgress_ > actKeyFrame_shot)
	{
		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(cursor_.x_ - GetGameObject()->transform_.localPosition_.x_,cursor_.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

		GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, aimSpeed);
		GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();
	}
	//撃つ
	if (actProgress_ > actKeyFrame_postAtk)
	{
		shotInterval_--;
		if(shotInterval_ <= 0)
		{
			ADXObject* projectileObj = ADXObject::Create(
				GetGameObject()->transform_.TransformPoint({ 0,0,1 }),
				GetGameObject()->transform_.GetWorldRotation(),
				{ 1,1,1 },
				GetGameObject()->transform_.parent_);
			Projectile* projectile = projectileObj->AddComponent<Projectile>();

			projectile->SetData((GetGameObject()->transform_.localPosition_ - cursor_).Normalize() * -projectileSpeed,
				ADXImage::LoadADXImage("texture/Clex_projectile.png"),
				projectileLifeTime);
			projectile->SetTeam(GetTeam());
			
			shotInterval_ = maxShotInterval;
		}
	}
	//後隙の後に待機フェーズへ
	else if (actProgress_ <= 0)
	{
		phase_ = phase::idol;
	}
}