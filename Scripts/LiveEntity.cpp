#include "LiveEntity.h"
#include "ADXCamera.h"
#include <time.h>

std::vector<AttackObject> LiveEntity::S_attackObjs = {};
std::vector<AttackObject> LiveEntity::S_allAttackObj = {};


void LiveEntity::Initialize(const std::string& setTeam)
{
	team = setTeam;
}

void LiveEntity::UniqueInitialize()
{
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;

	rect = ADXModel::CreateRect();
	gaugeTex = ADXImage::LoadADXImage("texture/whiteDot.png");

	visual = ADXObject::Create();
	visual->transform_.parent_ = &GetGameObject()->transform_;

	hpGaugeBG = ADXObject::Create();
	hpGaugeBG->transform_.parent_ = &GetGameObject()->transform_;
	hpGaugeBG->transform_.UpdateMatrix();
	hpGaugeBG->model_ = &rect;
	hpGaugeBG->texture_ = gaugeTex;
	hpGaugeBG->renderLayer_ = 1;

	hpGauge = ADXObject::Create();
	hpGauge->transform_.parent_ = &hpGaugeBG->transform_;
	hpGauge->transform_.UpdateMatrix();
	hpGauge->model_ = &rect;
	hpGauge->texture_ = gaugeTex;
	hpGauge->renderLayer_ = 2;

	particle = GetGameObject()->AddComponent<ADXParticleSystem>();
	particle->animation_.Initialize({
		ADXImage::LoadADXImage("texture/particle_defeat/000.png"), ADXImage::LoadADXImage("texture/particle_defeat/001.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/002.png"), ADXImage::LoadADXImage("texture/particle_defeat/003.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/004.png"), ADXImage::LoadADXImage("texture/particle_defeat/005.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/006.png"), ADXImage::LoadADXImage("texture/particle_defeat/007.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/008.png"), ADXImage::LoadADXImage("texture/particle_defeat/009.png"),
		ADXImage::LoadADXImage("texture/particle_defeat/010.png"), }, 0, false);
	particle->lifeTime_ = particle->animation_.GetLength();
	particle->particleModel_ = rect;

	LiveEntitiesInitialize();
}

void LiveEntity::UniqueUpdate()
{
	GetGameObject()->isVisible_ = true;

	hpGaugeBG->material_.ambient_ = { 0.1f,0.1f,0.1f };
	hpGauge->material_.ambient_ = { 0.1f,1,0.3f };

	hpGaugeBG->transform_.localPosition_ = { 0,-1.5f,0 };
	hpGauge->transform_.localPosition_ = { (1 - hpAmount),0,0 };
	hpGauge->transform_.localScale_ = { hpAmount,1,1 };

	visual->material_.ambient_ = { 1,1,1 };
	for (auto& itr : bodyParts)
	{
		itr->material_.ambient_ = { 1,1,1 };
	}

	visual->transform_.localPosition_ *= 0.8f;

	if (IsLive())
	{
		LiveEntitiesUpdate();
		hpGaugeBG->transform_.localScale_ = { 0.5,0.2f,1 };
	}
	else
	{
		DeadUpdate();
		hpGaugeBG->transform_.localScale_ = { 0.5,max(0,hpGaugeBG->transform_.localScale_.y_ - 0.03f),1};

		if (attackHitted)
		{
			particle->animation_.delayFrame_ = 0;
			particle->lifeTime_ = particle->animation_.GetLength();
			for (int i = 0; i < 10; i++)
			{
				particle->Emission();
				particle->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.3f;
				float particleScale = 1.5f + (float)(rand() % 5) * 0.1f;
				particle->particles_.back()->GetGameObject()->transform_.localScale_ = { particleScale ,particleScale ,particleScale };
				particle->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			particle->animation_.delayFrame_ = 3;
			particle->lifeTime_ = particle->animation_.GetLength() * 4;
			for (int i = 0; i < 30; i++)
			{
				particle->Emission();
				particle->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.6f;
				float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
				particle->particles_.back()->GetGameObject()->transform_.localScale_ = { particleScale ,particleScale ,particleScale };
				particle->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			damageSE.Play();
		}
	}

	for (auto& itr : particle->particles_)
	{
		itr->moveVec_ *= 0.9f;
		itr->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform_.modelRotation_;
	}

	if (attackHitted)
	{
		particle->animation_.delayFrame_ = 0;
		particle->lifeTime_ = particle->animation_.GetLength();
		for (int i = 0; i < 3; i++)
		{
			particle->Emission();
			particle->particles_.back()->GetGameObject()->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
			particle->particles_.back()->moveVec_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.1f;
			float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
			particle->particles_.back()->GetGameObject()->transform_.localScale_ = { particleScale ,particleScale ,particleScale };
			particle->particles_.back()->GetGameObject()->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
		}
		visual->material_.ambient_ = { 1,0,0 };
		for (auto& itr : bodyParts)
		{
			itr->material_.ambient_ = { 1,0,0 };
		}

		visual->transform_.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5)}.Normalize() * 0.3f;
		hpGaugeBG->transform_.localPosition_ = { 0,-1.5f + (float)sin(clock()) * 0.05f,0};
		attackHitted = false;
		damageSE.Play();
	}
}

void LiveEntity::Damage(float damage)
{
	if (IsLive() && !attackHitted)
	{
		hpAmount -= damage / maxHP;
		attackHitted = true;
	}
}

void LiveEntity::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if (!myCol->isTrigger_)
	{
		for (auto& objItr : GetAttackObj())
		{
			if (objItr.col != nullptr && objItr.attacker != nullptr
				&& col == objItr.col && objItr.attacker->team != team)
			{
				Damage(objItr.power);
			}
		}
	}
	LiveEntitiesOnCollisionHit(col, myCol);
}

void LiveEntity::OnPreRender()
{
	hpGaugeBG->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
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