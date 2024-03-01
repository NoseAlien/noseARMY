#include "Crazer_Unown.h"
#include "UnownFoot.h"
#include "Projectile.h"

const int maxFootsNum = 8;
const float footRadius = 2;
const float footStepRange = 0.15f;
const int footStepNum = 6;
const ADXVector3 launcherLocalPos = { 0,0,1 };
const float projectileSpeed = 2;
const float projectileAttackPower = 5;
const int maxShotInterval = 6;
const int projectileLifeTime = 90;
const float aimSpeed = 0.1f;
const float actProgressSpeed = 0.002f;
const float actKeyFrame_shot = 0.7f;
const float actKeyFrame_postAtk = 0.2f;
const float actKeyFrame_walk = 0.8f;
const float actKeyFrame_postWalk = 0.1f;

void Crazer_Unown::EnemyInitialize()
{
	nutralTex_ = ADXImage::LoadADXImage("texture/tempBossTex.png");
	deadTex_ = ADXImage::LoadADXImage("texture/tempBossTex.png");

	//足りない分の足を生成
	size_t needSpawnFootNum = maxFootsNum - foots_.size();
	size_t currentFootsNum = foots_.size();
	for (int i = 0; i < needSpawnFootNum; i++)
	{
		float footRad = ADXUtility::Pi * 2 / maxFootsNum * currentFootsNum + i;
		ADXVector3 footLocalPos = ADXVector3{ sinf(footRad),0,cosf(footRad) } *footRadius;

		ADXObject* footObj = ADXObject::Create(
			GetGameObject()->transform_.TransformPoint(footLocalPos),
			GetGameObject()->transform_.GetWorldRotation(),
			{ 1,1,1 },
			GetGameObject()->transform_.parent_);
		foots_.push_back(footObj->AddComponent<UnownFoot>());
		foots_.back()->SetParent(this);
	}
}

void Crazer_Unown::EnemyUpdate()
{
	scale_ = 20;

	if (!foots_.empty())
	{
		ADXVector3 bodyWorldPoint{};
		for (auto& itr : foots_)
		{
			bodyWorldPoint += itr->GetGameObject()->transform_.GetWorldPosition();
		}
		bodyWorldPoint /= (float)foots_.size();

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
	if (actProgress_ <= actKeyFrame_walk && actProgress_ > actKeyFrame_postWalk)
	{
		float walkProgress = ADXUtility::ValueMapping(actProgress_, actKeyFrame_walk, actKeyFrame_postWalk, 1, 0);

		//足りない分の足を生成
		size_t needSpawnFootNum = maxFootsNum - foots_.size();
		size_t currentFootsNum = foots_.size();
		for (int i = 0; i < needSpawnFootNum; i++)
		{
			float footRad = ADXUtility::Pi * 2 / maxFootsNum * currentFootsNum + i;
			ADXVector3 footLocalPos = ADXVector3{ sinf(footRad),0,cosf(footRad) } * footRadius;

			ADXObject* footObj = ADXObject::Create(
				GetGameObject()->transform_.TransformPoint(footLocalPos),
				GetGameObject()->transform_.GetWorldRotation(),
				{ 1,1,1 },
				GetGameObject()->transform_.parent_);
			foots_.push_back(footObj->AddComponent<UnownFoot>());
			foots_.back()->SetParent(this);
		}

		float stepLoopProgress = fmodf(walkProgress * footStepNum, 1);
		float oneStepProgress = fmodf(stepLoopProgress * 2, 1);
		bool walkOddIndexFoot = stepLoopProgress >= 0.5f;

		//自機の方を向く
		ADXVector3 targetRelativePos = targetPos_;
		if (GetGameObject()->transform_.parent_ != nullptr)
		{
			targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
		}

		ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
			{ 0,(float)atan2(targetRelativePos.x_ - GetGameObject()->transform_.localPosition_.x_,targetRelativePos.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

		GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, aimSpeed);
		GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();

		//歩く
		int index = 0;
		for (auto& itr : foots_)
		{
			if ((walkOddIndexFoot && index % 2 != 0) || (!walkOddIndexFoot && index % 2 == 0))
			{
				bool attack = false;
				float footRad = ADXUtility::Pi * 2 / foots_.size() * index;
				ADXVector3 footLocalPos = ADXVector3{ sinf(footRad),0,cosf(footRad) } * footRadius
					+ ADXVector3{ 0,0,footStepRange };

				if (oneStepProgress <= 0.2f)
				{
					attack = true;
					footLocalPos = GetGameObject()->transform_.InverseTransformPoint(
						itr->GetGameObject()->transform_.GetWorldPosition());
					footLocalPos.y_ = -1;
				}

				itr->PlayWalkPhase(GetGameObject()->transform_.TransformPoint(footLocalPos), attack, 0.5f,0.2f);
			}

			index++;
		}

		prevWalkOddIndexFoot_ = walkOddIndexFoot;
	}
	else if(actProgress_ <= 0)
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
	ADXVector3 targetRelativePos = targetPos_;
	if (GetGameObject()->transform_.parent_ != nullptr)
	{
		targetRelativePos = ADXMatrix4::Transform(targetPos_, GetGameObject()->transform_.parent_->GetMatWorld());
	}

	ADXQuaternion targetRot = ADXQuaternion::EulerToQuaternion(
		{ 0,(float)atan2(targetRelativePos.x_ - GetGameObject()->transform_.localPosition_.x_,targetRelativePos.z_ - GetGameObject()->transform_.localPosition_.z_),0 });

	GetGameObject()->transform_.localRotation_ = ADXQuaternion::Slerp(GetGameObject()->transform_.localRotation_, targetRot, aimSpeed);
	GetGameObject()->transform_.localRotation_ = GetGameObject()->transform_.localRotation_.Normalized();

	if (actProgress_ > actKeyFrame_shot)
	{
		
	}
	//撃つ
	else if (actProgress_ > actKeyFrame_postAtk)
	{
		shotInterval_--;
		if(shotInterval_ <= 0)
		{
			ADXVector3 launcherWorldPos =
				GetGameObject()->transform_.TransformPoint(launcherLocalPos);

			ADXObject* projectileObj = ADXObject::Create(
				launcherWorldPos,
				GetGameObject()->transform_.GetWorldRotation(),
				{ 1,1,1 },
				GetGameObject()->transform_.parent_);
			Projectile* projectile = projectileObj->AddComponent<Projectile>();

			projectile->SetData((launcherWorldPos - targetPos_).Normalize() * -projectileSpeed,
				ADXImage::LoadADXImage("texture/Crazer_Unown_projectile.png"),
				projectileLifeTime, projectileAttackPower);
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