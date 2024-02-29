#include "UnownFoot.h"

const float gravity = -1;
const float attackPower = 30;

void UnownFoot::SetParent(LiveEntity* setParent)
{
	parent_ = setParent;
	SetTeam(parent_->GetTeam());
}

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
	nutralTex_ = ADXImage::LoadADXImage("texture/tempBossFootTex.png");
	deadTex_ = ADXImage::LoadADXImage("texture/tempBossFootTex.png");
	attackTex_ = ADXImage::LoadADXImage("texture/whiteDot.png");

	enemyModel_ = ADXModel::LoadADXModel("model/UnownFoot.obj");

	visual_->model_ = &enemyModel_;
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

	//足の持ち主がいない、もしくは死んでいたら自爆する
	if (parent_ == nullptr || !parent_->IsLive())
	{
		Detonate();
	}
	else
	{
		GetGameObject()->transform_.SetWorldRotation(parent_->GetGameObject()->transform_.GetWorldRotation());
		GetGameObject()->transform_.UpdateMatrix();

		ADXVector3 parentRelativePos = parent_->GetGameObject()->transform_.GetWorldPosition();
			parentRelativePos = GetGameObject()->transform_.InverseTransformPoint(parentRelativePos);

		GetGameObject()->transform_.localRotation_ =
			ADXQuaternion::MakeAxisAngle(ADXVector3{0,1,0}, atan2(parentRelativePos.x_, parentRelativePos.z_))
			* GetGameObject()->transform_.localRotation_;
	}
}

void UnownFoot::EnemySafetyPhase()
{
	if (parent_ != nullptr && parent_->GetGameObject()->GetDeleteFlag())
	{
		parent_ = nullptr;
	}
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
