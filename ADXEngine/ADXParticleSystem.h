#pragma once
#include "ADXComponent.h"
#include "ADXParticle.h"
#include "ADXTexAnimation.h"
#include <list>

//パーティクルを出すオブジェクトに入れるコンポーネント
class ADXParticleSystem : public ADXComponent
{
public:
	ADXWorldTransform* parent_ = nullptr;
	bool loopEmission_ = false;
	uint32_t burstParticleNum_ = 1;
	uint32_t coolTime_ = 0;
	uint32_t maxParticleNum_ = 100;
	float scale_ = 1;
	int32_t lifeTime_ = 10;
	ADXTexAnimation animation_{};
	ADXModel* particleModel_ = nullptr;
	std::list<ADXParticle*> particles_{};
	bool billboard_ = true;

private:
	int32_t nowCoolTime_ = 0;

public:
	//パーティクルを生成
	void Emission();

private:
	//初期化
	void UniqueInitialize();

	//消えそうなインスタンスを参照していたらここで切る
	void SafetyPhase();
};