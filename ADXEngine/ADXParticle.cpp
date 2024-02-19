#include "ADXParticle.h"
#include "ADXCamera.h"

void ADXParticle::UniqueInitialize()
{
	lifeTime_ = maxLifeTime_;
}

void ADXParticle::UniqueUpdate()
{
	//移動
	GetGameObject()->transform_.localPosition_ += velocity_;
	//アニメーションの状態を更新
	animation_.Update();
	GetGameObject()->texture_ = animation_.GetNowTex();

	//基礎スケールと拡大率に応じてスケーリング
	float scale = scale_ * scaleRate_;
	GetGameObject()->transform_.localScale_ = { scale ,scale ,scale };

	//寿命を減らし、尽きたら消す
	lifeTime_--;
	if (lifeTime_ <= 0)
	{
		GetGameObject()->Destroy();
	}
}

void ADXParticle::OnPreRender()
{
	//必要に応じてビルボードにする
	if (billboard_)
	{
		GetGameObject()->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	}
}
