#include "LiveEntity.h"
#include "ADXCamera.h"
#include <time.h>

std::vector<AttackObject> LiveEntity::S_attackObjs = {};
std::vector<AttackObject> LiveEntity::S_allAttackObj = {};


void LiveEntity::Initialize(const std::string& setTeam)
{
	team_ = setTeam;
}

void LiveEntity::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;

	rect_ = ADXModel::CreateRect();
	gaugeTex_ = ADXImage::LoadADXImage("texture/whiteDot.png");

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
				particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle_->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.3f;
				float particleScale = 1.5f + (float)(rand() % 5) * 0.1f;
				particle_->particles_.back()->GetGameObject()->transform_.localScale_ = { particleScale ,particleScale ,particleScale };
				particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			particle_->animation_.delayFrame_ = 3;
			particle_->lifeTime_ = particle_->animation_.GetLength() * 4;
			for (int i = 0; i < 30; i++)
			{
				particle_->Emission();
				particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle_->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.6f;
				float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
				particle_->particles_.back()->GetGameObject()->transform_.localScale_ = { particleScale ,particleScale ,particleScale };
				particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			damageSE_.Play();
		}
	}

	for (auto& itr : particle_->particles_)
	{
		itr->moveVec_ *= 0.9f;
		itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform_.modelRotation_;
	}

	if (attackHitted_)
	{
		particle_->animation_.delayFrame_ = 0;
		particle_->lifeTime_ = particle_->animation_.GetLength();
		for (int i = 0; i < 3; i++)
		{
			particle_->Emission();
			particle_->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
			particle_->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.1f;
			float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
			particle_->particles_.back()->GetGameObject()->transform_.localScale_ = { particleScale ,particleScale ,particleScale };
			particle_->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
		}
		visual_->material_.ambient_ = { 1,0,0 };
		for (auto& itr : bodyParts_)
		{
			itr->material_.ambient_ = { 1,0,0 };
		}

		visual_->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5)}.Normalize() * 0.3f;
		hpGaugeBG_->transform_.localPosition_ = { 0,-1.5f + (float)sin(clock()) * 0.05f,0};
		attackHitted_ = false;
		damageSE_.Play();
	}
}

void LiveEntity::Damage(float damage)
{
	if (IsLive() && !attackHitted_)
	{
		hpAmount_ -= damage / maxHP_;
		attackHitted_ = true;
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
				Damage(objItr.power);
			}
		}
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