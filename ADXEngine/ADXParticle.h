#pragma once
#include "ADXObject.h"
#include "ADXTexAnimation.h"

//パーティクルとして扱うオブジェクトに入れるコンポーネント
class ADXParticle : public ADXComponent
{
public:
	ADXVector3 velocity_{};
	ADXQuaternion rotate_{};
	float scale_ = 1;
	float scaleRate_ = 1;
	uint32_t maxLifeTime_ = 0;
	int32_t lifeTime_ = 0;
	ADXTexAnimation animation_{};
	bool billboard_ = true;

private:
	//初期化処理
	void UniqueInitialize();

	//更新処理
	void UniqueUpdate();

	//描画前処理
	void OnPreRender();
};