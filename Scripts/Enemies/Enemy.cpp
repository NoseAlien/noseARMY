#include "Enemies/Enemy.h"
#include "ADXUtility.h"

const float scarchRadius = 12;
const float gravityScale = 0.015f;
const float drag = 0.8f;
const float grabbedSpeed = 0.1f;
const int32_t lowCarcassLifeTimeDivNum = 4;
const int32_t maxCarcassAnimationFrame = 1000;
const int32_t maxCarcassAnimationFrame_lowLifeTime = 200;
const int32_t carcassBlackOutFrame = 100;
const DirectX::XMFLOAT3 darkColor = { 0.2f,0.2f,0.2f };

void Enemy::LiveEntitiesInitialize()
{
	damageSE_->LoadADXAudio("sound/knock.wav");
	defeatSE_->LoadADXAudio("sound/slap.wav");

	enemyModel_ = ADXModel::LoadADXModel("model/groundBlock.obj");

	visual_->GetComponent<ADXModelRenderer>()->model_ = &enemyModel_;
	visual_->GetComponent<ADXModelRenderer>()->texture_ = ADXImage::LoadADXImage("texture/battleField.png");

	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->isTrigger_ = true;
	tempCol->colType_ = ADXCollider::sphere;
	tempCol->radius_ = scarchRadius;

	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();
	EnemyInitialize();
}

void Enemy::LiveEntitiesUpdate()
{
	rigidbody_->VelocityMove();

	rigidbody_->drag_ = drag;
	rigidbody_->dragAxis_ = { true,false,true };
	rigidbody_->gravity_ = { 0,-1,0 };
	rigidbody_->gravityScale_ = gravityScale;

	scale_ = 1;

	visual_->GetComponent<ADXModelRenderer>()->texture_ = nutralTex_;

	EnemyUpdate();

	GetGameObject()->transform_.localScale_ = { scale_,scale_,scale_ };

	targetDetected_ = false;
	carcassLifeTime_ = maxCarcassLifeTime;
}

void Enemy::DeadUpdate()
{
	rigidbody_->drag_ = drag;
	rigidbody_->dragAxis_ = { true,false,true };
	rigidbody_->gravity_ = { 0,-1,0 };
	rigidbody_->gravityScale_ = gravityScale;
	rigidbody_->VelocityMove();

	for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
	{
		itr->pushBackPriority_ = -2;
	}

	visual_->GetComponent<ADXModelRenderer>()->texture_ = deadTex_;

	//死骸を点滅させる、消えそうなときは早く点滅
	if (clock() % maxCarcassAnimationFrame < carcassBlackOutFrame
		|| (carcassLifeTime_ <= (int32_t)maxCarcassLifeTime / lowCarcassLifeTimeDivNum
			&& clock() % maxCarcassAnimationFrame_lowLifeTime < carcassBlackOutFrame))
	{
		for (auto& itr : bodyParts_)
		{
			itr->GetComponent<ADXModelRenderer>()->material_.ambient_ = darkColor;
		}
	}

	//自機の分身が死骸を掴んでいる時の処理
	if (grabber_ != nullptr)
	{
		GetGameObject()->transform_.SetWorldPosition(GetGameObject()->transform_.GetWorldPosition()
			+ (grabber_->GetGameObject()->transform_.GetWorldPosition() - GetGameObject()->transform_.GetWorldPosition()) * grabbedSpeed);

		for (auto& itr : GetGameObject()->GetComponents<ADXCollider>())
		{
			if (!itr->isTrigger_)
			{
				//自分のコライダーを攻撃判定があるものとして登録
				LiveEntity::SetAttackObj({ itr,(LiveEntity*)grabber_->GetParent(),maxHP_ });
			}
		}
	}

	//死骸の寿命を減らし、無くなったら消す
	carcassLifeTime_--;
	if (carcassLifeTime_ <= 0)
	{
		GetGameObject()->Destroy();
	}
}

void Enemy::LiveEntitiesOnCollisionHit(ADXCollider* col, ADXCollider* myCol)
{
	if(myCol->isTrigger_ && IsLive())
	{
		LiveEntity* tempLiv = col->GetGameObject()->GetComponent<LiveEntity>();
		if (!col->isTrigger_ && tempLiv != nullptr && tempLiv->IsLive() && tempLiv->GetTeam() != GetTeam())
		{
			targetDetected_ = true;
			targetPos_ = col->GetGameObject()->transform_.GetWorldPosition();
		}
	}
	else if(!myCol->isTrigger_ && !IsLive() && grabber_ == nullptr)
	{
		//死骸になっているときに自機の分身に触れたら掴まれたものする、掴んでいる死骸に触れた時も同様
		if (col->GetGameObject()->GetComponent<PlayerMini>())
		{
			grabber_ = col->GetGameObject()->GetComponent<PlayerMini>();
		}
		if (!col->isTrigger_ && col->GetGameObject()->GetComponent<Enemy>()
			&& col->GetGameObject()->GetComponent<Enemy>()->grabber_ != nullptr)
		{
			grabber_ = col->GetGameObject()->GetComponent<Enemy>()->grabber_;
		}
	}
}

void Enemy::SafetyPhase()
{
	//死骸の時、自身を掴んでいた自機の分身が消えたらポインタをnullptrに
	if (grabber_ != nullptr && grabber_->GetGameObject()->GetDeleteFlag())
	{
		grabber_ = nullptr;
	}
	EnemySafetyPhase();
}