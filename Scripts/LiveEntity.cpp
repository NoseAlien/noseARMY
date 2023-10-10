#include "LiveEntity.h"
#include "ADXCamera.h"
#include <time.h>

std::vector<LiveEntity*> LiveEntity::S_liveEntities = {};
std::vector<LiveEntity*> LiveEntity::S_allLiveEntitiesPtr = {};
std::vector<AttackObject> LiveEntity::S_attackObjs = {};
std::vector<AttackObject> LiveEntity::S_allAttackObj = {};


void LiveEntity::Initialize(const std::string& setTeam)
{
	team = setTeam;

	rect = ADXModel::CreateRect();
	gaugeTex = ADXImage::LoadADXImage("whiteDot.png");

	hpGaugeBG = ADXObject::Create();
	hpGaugeBG->transform.parent_ = &GetGameObject()->transform;
	hpGaugeBG->transform.UpdateMatrix();
	hpGaugeBG->model = &rect;
	hpGaugeBG->texture = gaugeTex;
	hpGaugeBG->renderLayer = 1;

	hpGauge = ADXObject::Create();
	hpGauge->transform.parent_ = &hpGaugeBG->transform;
	hpGauge->transform.UpdateMatrix();
	hpGauge->model = &rect;
	hpGauge->texture = gaugeTex;
	hpGauge->renderLayer = 2;

	particle = GetGameObject()->AddComponent<ADXParticleSystem>();
	particle->animation.Initialize({
		ADXImage::LoadADXImage("particle_defeat/000.png"), ADXImage::LoadADXImage("particle_defeat/001.png"),
		ADXImage::LoadADXImage("particle_defeat/002.png"), ADXImage::LoadADXImage("particle_defeat/003.png"),
		ADXImage::LoadADXImage("particle_defeat/004.png"), ADXImage::LoadADXImage("particle_defeat/005.png"),
		ADXImage::LoadADXImage("particle_defeat/006.png"), ADXImage::LoadADXImage("particle_defeat/007.png"),
		ADXImage::LoadADXImage("particle_defeat/008.png"), ADXImage::LoadADXImage("particle_defeat/009.png"),
		ADXImage::LoadADXImage("particle_defeat/010.png"), }, 0, false);
	particle->lifeTime = particle->animation.GetLength();
	particle->particleModel = rect;
}

void LiveEntity::UniqueInitialize()
{
	LiveEntitiesInitialize();

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
}

void LiveEntity::UniqueUpdate()
{
	GetGameObject()->isVisible = true;

	hpGaugeBG->material.ambient = { 0.1f,0.1f,0.1f };
	hpGauge->material.ambient = { 0.1f,1,0.3f };

	hpGaugeBG->transform.localPosition_ = { 0,-1.5f,0 };
	hpGauge->transform.localPosition_ = { (1 - hpAmount),0,0 };
	hpGauge->transform.localScale_ = { hpAmount,1,1 };

	GetGameObject()->material.ambient = { 1,1,1 };

	GetGameObject()->transform.modelPosition_ *= 0.8f;

	if (IsLive())
	{
		LiveEntitiesUpdate();
		hpGaugeBG->transform.localScale_ = { 0.5,0.2f,1 };
	}
	else
	{
		DeadUpdate();
		hpGaugeBG->transform.localScale_ = { 0.5,max(0,hpGaugeBG->transform.localScale_.y - 0.03f),1};

		if (attackHitted)
		{
			particle->animation.delayFrame = 0;
			particle->lifeTime = particle->animation.GetLength();
			for (int i = 0; i < 10; i++)
			{
				particle->Emission();
				particle->particles.back()->GetGameObject()->transform.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle->particles.back()->moveVec = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.3f;
				float particleScale = 1.5f + (float)(rand() % 5) * 0.1f;
				particle->particles.back()->GetGameObject()->transform.localScale_ = { particleScale ,particleScale ,particleScale };
				particle->particles.back()->GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			particle->animation.delayFrame = 3;
			particle->lifeTime = particle->animation.GetLength() * 4;
			for (int i = 0; i < 30; i++)
			{
				particle->Emission();
				particle->particles.back()->GetGameObject()->transform.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle->particles.back()->moveVec = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.6f;
				float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
				particle->particles.back()->GetGameObject()->transform.localScale_ = { particleScale ,particleScale ,particleScale };
				particle->particles.back()->GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
		}
	}

	particle->Update(GetGameObject());
	for (auto& itr : particle->particles)
	{
		itr->moveVec *= 0.9f;
		itr->GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }) * itr->GetGameObject()->transform.modelRotation_;
	}

	if (attackHitted)
	{
		particle->animation.delayFrame = 0;
		particle->lifeTime = particle->animation.GetLength();
		for (int i = 0; i < 3; i++)
		{
			particle->Emission();
			particle->particles.back()->GetGameObject()->transform.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
			particle->particles.back()->moveVec = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.1f;
			float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
			particle->particles.back()->GetGameObject()->transform.localScale_ = { particleScale ,particleScale ,particleScale };
			particle->particles.back()->GetGameObject()->transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
		}
		GetGameObject()->material.ambient = { 1,0,0 };
		GetGameObject()->transform.modelPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5)}.Normalize() * 0.3f;
		hpGaugeBG->transform.localPosition_ = { 0,-1.5f + (float)sin(clock()) * 0.05f,0};
		attackHitted = false;
	}

	S_allLiveEntitiesPtr.push_back(this);
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
	if (!myCol->isTrigger)
	{
		for (auto& objItr : GetAttackObj())
		{
			if (col == objItr.col && objItr.attacker->team != team)
			{
				Damage(objItr.power);
			}
		}
	}
	LiveEntitiesOnCollisionHit(col, myCol);
}

void LiveEntity::OnPreRender()
{
	hpGaugeBG->transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform.GetWorldRotation());
}

void LiveEntity::StaticUpdate()
{
	S_liveEntities = S_allLiveEntitiesPtr;
	S_allLiveEntitiesPtr.clear();
	S_attackObjs = S_allAttackObj;
	S_allAttackObj.clear();
}

void LiveEntity::SetAttackObj(const AttackObject& attackObj)
{
	S_allAttackObj.push_back(attackObj);
}