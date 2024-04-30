#pragma once

#include "ADXObject.h"
#include "ADXRigidbody.h"

class Player;

//自機の分身のオブジェクトに入れるコンポーネント
class PlayerMini : public ADXComponent
{
private:
	ADXObject* nose_ = nullptr;
	ADXObject* body_ = nullptr;
	ADXObject* shadow_ = nullptr;
	ADXModel rect_{};
	ADXModel shadowModel_{};
	uint32_t shadowTex_ = 0;

	Player* parent_ = nullptr;

	ADXQuaternion targetRot_{};
	ADXRigidbody* rigidbody_ = nullptr;
	float bodyRotAngle_ = 0;
	bool destroyFlag_ = false;

public:
	//追従する自機を設定
	void Initialize(Player* setParent);

	//追従する自機を取得
	Player* GetParent() { return parent_; };
		
private:
	//---以下の関数は必要な時に自動で呼び出される---

	//初期化処理
	void UniqueInitialize() final;

	//更新処理
	void UniqueUpdate() final;

	//描画前処理
	void OnPreRender() final;

	//---以上の関数は必要な時に自動で呼び出される---

	//移動
	void Move(float walkSpeed, float jumpPower);
};