#include "UnownFoot.h"

const float gravity = -1;
const float attackPower = 30;

void UnownFoot::PlayWalkPhase(const ADXVector3& setTargetPoint, bool setAttack,
	float setActProgressSpeed, float setWalkSpeed)
{
	targetPoint_ = setTargetPoint;
	attack_ = setAttack;
	actProgressSpeed_ = setActProgressSpeed;
	walkSpeed_ = setWalkSpeed;

	actProgress_ = 1;
	phase_ = phase::walk;
}

void UnownFoot::EnemyInitialize()
{
	nutralTex_ = ADXImage::LoadADXImage("texture/tempBossTex.png");
	attackTex_ = ADXImage::LoadADXImage("texture/whiteDot.png");
}

void UnownFoot::EnemyUpdate()
{
	rigidbody_->gravity_ = { 0,gravity,0 };
	scale_ = 6;

	switch (phase_)
	{
	case phase::walk:
		Walk();
		break;
	default:
		Idol();
		break;
	}

	actProgress_ = min(max(0, actProgress_ - actProgressSpeed_), 1);
}

void UnownFoot::Idol()
{
}

void UnownFoot::Walk()
{
	if (actProgress_ > 0)
	{
		GetGameObject()->GetComponent<ADXRigidbody>()->velocity_ =
			(targetPoint_ - GetGameObject()->transform_.GetWorldPosition()) * walkSpeed_;

		if (attack_)
		{
			visual_->texture_ = attackTex_;
			for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
			{
				if (!itr->isTrigger_)
				{
					//当たったらダメージを受けるオブジェクトとして登録
					LiveEntity::SetAttackObj({ itr,this,attackPower });
				}
			}
		}
	}
	else
	{
		phase_ = phase::idol;
	}
}
