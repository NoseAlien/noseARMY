#include "Species.h"
#include "ADXCamera.h"
#include <time.h>

std::vector<Species*> Species::S_species = {};
std::vector<Species*> Species::S_allSpeciesPtr = {};
std::vector<AttackObject> Species::S_attackObjs = {};
std::vector<AttackObject> Species::S_allAttackObj = {};


void Species::Initialize(const std::string& setTeam)
{
	team = setTeam;

	rect = ADXModel::CreateRect();
	gaugeTex = ADXImage::LoadADXImage("whiteDot.png");

	hpGaugeBG.Initialize();
	hpGaugeBG.transform.parent_ = &transform;
	hpGaugeBG.transform.UpdateMatrix();
	hpGaugeBG.model = &rect;
	hpGaugeBG.texture = gaugeTex;
	hpGaugeBG.renderLayer = 1;

	hpGauge.Initialize();
	hpGauge.transform.parent_ = &hpGaugeBG.transform;
	hpGauge.transform.UpdateMatrix();
	hpGauge.model = &rect;
	hpGauge.texture = gaugeTex;
	hpGauge.renderLayer = 2;

	particle.Initialize(this);
	particle.animation.Initialize({
		ADXImage::LoadADXImage("particle_defeat/000.png"), ADXImage::LoadADXImage("particle_defeat/001.png"),
		ADXImage::LoadADXImage("particle_defeat/002.png"), ADXImage::LoadADXImage("particle_defeat/003.png"),
		ADXImage::LoadADXImage("particle_defeat/004.png"), ADXImage::LoadADXImage("particle_defeat/005.png"),
		ADXImage::LoadADXImage("particle_defeat/006.png"), ADXImage::LoadADXImage("particle_defeat/007.png"),
		ADXImage::LoadADXImage("particle_defeat/008.png"), ADXImage::LoadADXImage("particle_defeat/009.png"),
		ADXImage::LoadADXImage("particle_defeat/010.png"), }, 0, false);
	particle.lifeTime = particle.animation.GetLength();
	particle.particleModel = rect;
}

void Species::UniqueUpdate()
{
	hpGaugeBG.material.ambient = { 0.1f,0.1f,0.1f };
	hpGauge.material.ambient = { 0.1f,1,0.3f };

	hpGaugeBG.transform.localPosition_ = { 0,-1.5f,0 };
	hpGauge.transform.localPosition_ = { (1 - hpAmount),0,0 };
	hpGauge.transform.localScale_ = { hpAmount,1,1 };

	material.ambient = { 1,1,1 };

	transform.modelPosition_ *= 0.8f;

	if (IsArrive())
	{
		SpeciesUpdate();
		hpGaugeBG.transform.localScale_ = { 0.5,0.2f,1 };
	}
	else
	{
		DeadUpdate();
		hpGaugeBG.transform.localScale_ = { 0.5,max(0,hpGaugeBG.transform.localScale_.y - 0.03f),1};

		if (attackHitted)
		{
			particle.animation.delayFrame = 0;
			particle.lifeTime = particle.animation.GetLength();
			for (int i = 0; i < 10; i++)
			{
				particle.Emission();
				particle.particles.back().transform.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle.particles.back().moveVec = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.3f;
				float particleScale = 1.5f + (float)(rand() % 5) * 0.1f;
				particle.particles.back().transform.localScale_ = { particleScale ,particleScale ,particleScale };
				particle.particles.back().transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
			particle.animation.delayFrame = 3;
			particle.lifeTime = particle.animation.GetLength() * 4;
			for (int i = 0; i < 30; i++)
			{
				particle.Emission();
				particle.particles.back().transform.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
				particle.particles.back().moveVec = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.6f;
				float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
				particle.particles.back().transform.localScale_ = { particleScale ,particleScale ,particleScale };
				particle.particles.back().transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
			}
		}
	}

	particle.Update(this);
	for (auto& itr : particle.particles)
	{
		itr.moveVec *= 0.9f;
		itr.transform.modelRotation_ = ADXQuaternion::Multiply(itr.transform.modelRotation_, ADXQuaternion::EulerToQuaternion({ 0,0,0.01f }));
	}

	if (attackHitted)
	{
		particle.animation.delayFrame = 0;
		particle.lifeTime = particle.animation.GetLength();
		for (int i = 0; i < 3; i++)
		{
			particle.Emission();
			particle.particles.back().transform.localPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize();
			particle.particles.back().moveVec = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5) }.Normalize() * 0.1f;
			float particleScale = 0.1f + (float)(rand() % 3) * 0.1f;
			particle.particles.back().transform.localScale_ = { particleScale ,particleScale ,particleScale };
			particle.particles.back().transform.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,(float)rand() });
		}
		material.ambient = { 1,0,0 };
		transform.modelPosition_ = ADXVector3{ (float)(rand() % 11 - 5),(float)(rand() % 11 - 5),(float)(rand() % 11 - 5)}.Normalize() * 0.3f;
		hpGaugeBG.transform.localPosition_ = { 0,-1.5f + (float)sin(clock()) * 0.05f,0};
		attackHitted = false;
	}

	hpGaugeBG.Update();
	hpGauge.Update();

	S_allSpeciesPtr.push_back(this);
}

void Species::Damage(float damage)
{
	if (IsArrive() && !attackHitted)
	{
		hpAmount -= damage / maxHP;
		attackHitted = true;
	}
}

void Species::SpeciesUpdate()
{
}

void Species::DeadUpdate()
{
}

void Species::SpeciesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
}

void Species::OnCollisionHit(ADXCollider* col, ADXCollider* myCol)
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
	SpeciesOnCollisionHit(col, myCol);
}

void Species::OnPreRender()
{
	hpGaugeBG.transform.SetWorldRotation(ADXCamera::GetCurrentCamera()->transform.GetWorldRotation());
}

void Species::StaticUpdate()
{
	S_species = S_allSpeciesPtr;
	S_allSpeciesPtr.clear();
	S_attackObjs = S_allAttackObj;
	S_allAttackObj.clear();
}

void Species::SetAttackObj(AttackObject attackObj)
{
	S_allAttackObj.push_back(attackObj);
}