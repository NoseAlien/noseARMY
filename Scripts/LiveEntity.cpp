#include "LiveEntity.h"
#include "ADXCamera.h"
#include <time.h>

const float LiveEntity::basicHP = 50;
const float LiveEntity::basicAttackPower = 10;
const float LiveEntity::repairPower = 0.005f;
const uint32_t LiveEntity::maxRepairCoolTime = 400;
const uint32_t LiveEntity::basicGhostTimeFrame = 40;
const uint32_t LiveEntity::reviveGhostTimeFrame = 60;

std::vector<LiveEntity::AttackObject> LiveEntity::S_attackObjs = {};
std::vector<LiveEntity::AttackObject> LiveEntity::S_allAttackObj = {};


void LiveEntity::SetTeam(const std::string& setTeam)
{
	team_ = setTeam;
}

void LiveEntity::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;

	rect_ = ADXModel::CreateRect();
	gaugeTex_ = ADXImage::LoadADXImage("texture/whiteDot.png");

	damageSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	defeatSE_ = GetGameObject()->AddComponent<ADXAudioSource>();

	visual_ = ADXObject::Create();
	visual_->transform_.parent_ = &GetGameObject()->transform_;

	hpGaugeBG_ = ADXObject::Create();
	hpGaugeBG_->transform_.parent_ = &GetGameObject()->transform_;
	hpGaugeBG_->transform_.UpdateMatrix();
	hpGaugeBG_->model_ = &rect_;
	hpGaugeBG_->texture_ = gaugeTex_;
	hpGaugeBG_->renderLayer_ = 1;

	hpGauge_ = ADXObject::Create();
	hpGauge_->transform_.parent_ = &hpGaugeBG_->transform_;
	hpGauge_->transform_.UpdateMatrix();
	hpGauge_->model_ = &rect_;
	hpGauge_->texture_ = gaugeTex_;
	hpGauge_->renderLayer_ = 2;

	particle_ = GetGameObject()->AddComponent<ADXParticleSystem>();
	particle_->animation_.Initialize({
		ADXImage::LoadADXImage("texture/particle_defeat/000.png"), ADXImage::LoadADXImage("texture/particle_defeat/001.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/002.png"), ADXImage::LoadADXImage("texture/particle_defeat/003.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/004.png"), ADXImage::LoadADXImage("texture/particle_defeat/005.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/006.png"), ADXImage::LoadADXImage("texture/particle_defeat/007.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/008.png"), ADXImage::LoadADXImage("texture/particle_defeat/009.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/010.png"), }, 0, false);
	particle_->lifeTime_ = particle_->animation_.GetLength();
	particle_->particleModel_ = rect_;

	LiveEntitiesInitialize();
}

void LiveEntity::UniqueUpdate()
{
	GetGameObject()->isVisible_ = true;
	
	if (IsLive() && ghostTime_ >= 0)
	{
		if (repairCoolTime_ > 0)
		{
			repairCoolTime_--;
		}
		else
		{
			hpAmount_ += repairPower;
		}
	}

	hpAmount_ = min(max(0, hpAmount_), 1);

	hpGaugeBG_->material_.ambient_ = { 0.1f,0.1f,0.1f };
	hpGauge_->material_.ambient_ = { 0.1f,1,0.3f };

	hpGaugeBG_->transform_.localPosition_ = { 0,-1.5f,0 };
	hpGauge_->transform_.localPosition_ = { (1 - hpAmount_),0,0 };
	hpGauge_->transform_.localScale_ = { hpAmount_,1,1 };

	visual_->material_.ambient_ = { 1,1,1 };
	for (auto& itr : bodyParts_)
	{
		itr->material_.ambient_ = { 1,1,1 };
	}

	visual_->transform_.localPosition_ *= 0.8f;

	if (IsLive())
	{
		LiveEntitiesUpdate();
		hpGaugeBG_->transform_.localScale_ = { 0.5,0.2f,1 };
	}
	else
	{
		DeadUpdate();
		hpGaugeBG_->transform_.localScale_ = { 0.5,max(0,hpGaugeBG_->transform_.localScale_.y_ - 0.03f),1};

		if (attackHitted_)
		{
			particle_->animation_.delayFrame_ = 0;
			particle_->lifeTime_ = particle_->animation_.GetLength();
			for (int i = 0; i < 10; i++)
			{
				particle_->Emission();
				particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
				particle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.3f;
				particle_->particles_.back()->scale_ = ADXUtility::RandomRange(3.5f, 4);
				particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			particle_->animation_.delayFrame_ = 3;
			particle_->lifeTime_ = particle_->animation_.GetLength() * 4;
			for (int i = 0; i < 30; i++)
			{
				particle_->Emission();
				particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
				particle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
				particle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.1f, 0.7f);
				particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			defeatSE_->Play();
		}
	}

	for (auto& itr : particle_->particles_)
	{
		itr->velocity_ *= 0.9f;
		itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform_.modelRotation_;
	}

	if (attackHitted_)
	{
		particle_->animation_.delayFrame_ = 0;
		particle_->lifeTime_ = particle_->animation_.GetLength();
		for (int i = 0; i < 2; i++)
		{
			particle_->Emission();
			particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
			particle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.1f;
			particle_->particles_.back()->scale_ = ADXUtility::RandomRange(1, 1.5f);
			particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
		}
		particle_->animation_.delayFrame_ = 1;
		particle_->lifeTime_ = particle_->animation_.GetLength() * 2;
		for (int i = 0; i < 6; i++)
		{
			particle_->Emission();
			particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize();
			particle_->particles_.back()->velocity_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5) }.Normalize() * 0.3f;
			particle_->particles_.back()->scale_ = ADXUtility::RandomRange(0.2f, 0.5f);
			particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
		}

		visual_->transform_.localPosition_ = ADXVector3{ ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5),ADXUtility::RandomRange(-5,5)}.Normalize() * 0.3f;
		hpGaugeBG_->transform_.localPosition_ = { 0,-1.5f + (float)sin(clock()) * 0.05f,0};
		attackHitted_ = false;
		damageSE_->Play();
	}

	if (IsLive() && ghostTime_ > 0 && ghostTime_ % 4 >= 2)
	{
		visual_->material_.ambient_ = { 1,0.2f,0.2f };
		for (auto& itr : bodyParts_)
		{
			itr->material_.ambient_ = { 1,0.2f,0.2f };
		}
	}

	ghostTime_ = max(0, ghostTime_ - 1);

	if (isOutOfField_ && latestHitField_ != nullptr)
	{
		GetGameObject()->transform_.SetWorldPosition(latestHitField_->GetGameObject()->GetComponent<ADXCollider>()->ClosestPoint(GetGameObject()->transform_.GetWorldPosition()));
	}

	isOutOfField_ = true;
}

void LiveEntity::Damage(float damage)
{
	if (IsLive() && !attackHitted_ && ghostTime_ <= 0)
	{
		hpAmount_ -= damage / maxHP_;
		ghostTime_ = int32_t(damage / basicAttackPower * basicGhostTimeFrame);
		repairCoolTime_ = maxRepairCoolTime;
		attackHitted_ = true;
	}
}

void LiveEntity::Revive()
{
	if (!IsLive())
	{
		hpAmount_ = 1;
		ghostTime_ = reviveGhostTimeFrame;
	}
}

void LiveEntity::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (!myCol->isTrigger_)
	{
		for (auto& objItr : GetAttackObj())
		{
			if (objItr.col != nullptr && objItr.attacker != nullptr
				&& col == objItr.col && objItr.attacker->team_ != team_)
			{
				bool prevIsLive = IsLive();
				Damage(objItr.power);
				if (!IsLive() && prevIsLive && objItr.attacker != nullptr)
				{
					objItr.attacker->killCount_++;
				}
			}
		}
	}

	if (col->GetGameObject()->GetComponent<FieldBox>() != nullptr
		&& (GetGameObject()->transform_.GetWorldPosition() - col->ClosestPoint(GetGameObject()->transform_.GetWorldPosition())).Length() < 0.1f)
	{
		isOutOfField_ = false;
		latestHitField_ = col->GetGameObject()->GetComponent<FieldBox>();
	}

	LiveEntitiesOnCollisionHit(col, myCol);
}

void LiveEntity::OnPreRender()
{
	hpGaugeBG_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	LiveEntitiesOnPreRender();
}

void LiveEntity::StaticUpdate()
{
	S_attackObjs = S_allAttackObj;
	S_allAttackObj.clear();
}

void LiveEntity::SetAttackObj(const AttackObject& attackObj)
{
	S_allAttackObj.push_back(attackObj);
}