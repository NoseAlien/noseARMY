#include "Pebble.h"
#include "ADXModelRenderer.h"
#include "ShadowRenderer.h"
#include "ADXCamera.h"

const float drag = 0.95f;
const ADXVector3 gravity = { 0,-0.01f,0 };
const int32_t defaultSortingOrder = 2;
const int32_t shadowSortingOrder = 1;

void Pebble::UniqueInitialize()
{
	//モデル読み込み
	rect_ = ADXModel::CreateRect();
	shadowModel_ = ADXModel::LoadADXModel("model/cylinder.obj");
	//画像読み込み
	billBoardTex_ = ADXImage::LoadADXImage("texture/pebble.png");
	shadowTex_ = ADXImage::LoadADXImage("texture/whiteDot.png");
	//音読み込み
	hitSE_ = GetGameObject()->AddComponent<ADXAudioSource>();
	hitSE_->LoadADXAudio("sound/knock.wav");
	hitSE_->useDistanceFade_ = true;

	//コライダー生成
	ADXCollider* tempCol = GetGameObject()->AddComponent<ADXCollider>();
	tempCol->pushable_ = true;
	tempCol->pushBackPriority_ = -2;
	//リジッドボディ生成
	rigidbody_ = GetGameObject()->AddComponent<ADXRigidbody>();
	rigidbody_->drag_ = drag;
	rigidbody_->gravity_ = gravity;
	rigidbody_->gravityScale_ = 1;
	//ビルボード用のオブジェクトを作成
	billBoard_ = ADXObject::Create();
	billBoard_->transform_.parent_ = &GetGameObject()->transform_;
	ADXModelRenderer* tempRenderer = billBoard_->AddComponent<ADXModelRenderer>();
	tempRenderer->model_ = &rect_;
	tempRenderer->texture_ = billBoardTex_;
	billBoard_->sortingOrder_ = defaultSortingOrder;
	//影のオブジェクトを作成
	shadow_ = ADXObject::Create();
	shadow_->transform_.localPosition_ = { 0,-6.5f,0 };
	shadow_->transform_.localScale_ = { 1,6,1 };
	shadow_->transform_.parent_ = &GetGameObject()->transform_;
	ShadowRenderer* tempShadowRenderer = shadow_->AddComponent<ShadowRenderer>();
	tempShadowRenderer->model_ = &shadowModel_;
	tempShadowRenderer->texture_ = shadowTex_;
	tempShadowRenderer->material_.ambient_ = { 0,0,0 };
	tempShadowRenderer->material_.alpha_ = 0.6f;
	shadow_->sortingOrder_ = shadowSortingOrder;

	prevPos_ = GetGameObject()->transform_.localPosition_;
	mute_ = true;
}

void Pebble::UniqueUpdate()
{
	rigidbody_->VelocityMove();

	if (!mute_ && rigidbody_->velocity_.Length() > prevVel_.Length()
		&& rigidbody_->velocity_.Normalize().Dot(prevVel_.Normalize()) <= 0.99f)
	{
		hitSE_->Play();
	}

	rotAngle_ = fmodf(rotAngle_ + (GetGameObject()->transform_.localPosition_ - prevPos_).Length(), ADXUtility::Pi * 2);

	prevPos_ = GetGameObject()->transform_.localPosition_;
	prevVel_ = rigidbody_->velocity_;

	mute_ = false;
}

void Pebble::OnPreRender()
{
	//ビルボード
	billBoard_->transform_.SetWorldRotation(ADXCamera::GetCurrentCamera()->GetGameObject()->transform_.GetWorldRotation());
	billBoard_->transform_.modelRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,rotAngle_ });
}
