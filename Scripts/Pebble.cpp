#include "Pebble.h"
#include "ADXCamera.h"

const float drag = 0.95f;
const ADXVector3 gravity = { 0,-0.01f,0 };

void Pebble::UniqueInitialize()
{
	//ビルボード用のモデル
	rect_ = ADXModel::CreateRect();
	//ビルボード用の画像
	billBoardTex_ = ADXImage::LoadADXImage("texture/pebble.png");
	//何かに弾かれた時の音
	hitSE_ = ADXAudio::LoadADXAudio("sound/knock.wav");

	//コライダー生成
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
	tempCol->pushBackPriority_ = -2;
	//リジッドボディ生成
	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();
	rigidbody_->drag_ = drag;
	rigidbody_->gravity_ = gravity;
	rigidbody_->gravityScale_ = 1;
	//ビルボード用のモデルを作成
	billBoard_ = ADXObject::Create();
	billBoard_->transform_.parent_ = &GetGameObject()->transform_;
	billBoard_->model_ = &rect_;
	billBoard_->texture_ = billBoardTex_;

	prevPos_ = GetGameObject()->transform_.localPosition_;
}

void Pebble::UniqueUpdate()
{
	rigidbody_->VelocityMove();

	if (rigidbody_->velocity_.Length() > prevVel_.Length()
		&& rigidbody_->velocity_.Normalize().Dot(prevVel_.Normalize()) <= 0.99f)
	{
		hitSE_.Play();
	}

	rotAngle_ = fmodf(rotAngle_ + (GetGameObject()->transform_.localPosition_ - prevPos_).Length(), ADXUtility::Pi * 2);

	prevPos_ = GetGameObject()->transform_.localPosition_;
	prevVel_ = rigidbody_->velocity_;
}

void Pebble::OnPreRender()
{
	//ビルボード
	billBoard_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	billBoard_->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,rotAngle_ });
}
