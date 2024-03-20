#include "StageSelectScene.h"
#include "SceneTransition.h"
#include "ADXModelRenderer.h"
#include "ADXKeyConfig.h"
#include "Pebble.h"

void StageSelectScene::Initialize()
{
	player_ = nullptr;
	floors_ = {};
	fields_ = {};
	tutorialAreas_ = {};

	keyImg_ = ADXImage::LoadADXImage("texture/QUIT_TITLE.png");
	backGroundTex_ = ADXImage::LoadADXImage("texture/skyBG.png");
	groundImg_ = ADXImage::LoadADXImage("texture/GroundBlock.png");

	rect_ = ADXModel::CreateRect();
	ground_ = ADXModel::LoadADXModel("model/groundBlock.obj");

	ADXObject* temp = ADXObject::Create();
	shutter_ = temp->AddComponent<SceneTransition>();


	temp = ADXObject::Create({ 0,2,0 });
	player_ = temp->AddComponent<Player>();
	player_->SetTeam("player");

	floors_.push_back(ADXObject::Create({ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 }));
	floors_.back()->GetComponent<ADXModelRenderer>()->model_ = &ground_;
	floors_.back()->GetComponent<ADXModelRenderer>()->texture_ = groundImg_;
	ADXCollider* tempCol = floors_.back()->AddComponent<ADXCollider>();
	tempCol->colType_ = ADXCollider::box;

	floors_.push_back(ADXObject::Create({ 0,-2,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 50,1,50 }));
	floors_.back()->GetComponent<ADXModelRenderer>()->model_ = &ground_;
	floors_.back()->GetComponent<ADXModelRenderer>()->texture_ = groundImg_;
	tempCol = floors_.back()->AddComponent<ADXCollider>();
	tempCol->colType_ = ADXCollider::box;

	temp = ADXObject::Create({ 12,1,18 }, ADXQuaternion::IdentityQuaternion(), { 0.5f,0.5f,0.5f });
	temp->AddComponent<Pebble>();

	temp = ADXObject::Create({ -15,1,-12 }, ADXQuaternion::IdentityQuaternion(), { 1,1,1 });
	temp->AddComponent<Pebble>();

	temp = ADXObject::Create({ 20,1,-14 }, ADXQuaternion::IdentityQuaternion(), { 0.7f,0.7f,0.7f });
	temp->AddComponent<Pebble>();

	temp = ADXObject::Create({ -22,1,30 }, ADXQuaternion::IdentityQuaternion(), { 0.3f,0.3f,0.3f });
	temp->AddComponent<Pebble>();


	temp = ADXObject::Create({ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_move.png"));

	temp = ADXObject::Create({ 0,13,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 50,14,50 });
	fields_.push_back(temp->AddComponent<FieldBox>());


	temp = ADXObject::Create({ 0,1.01f,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 });
	gates_.push_back(temp->AddComponent<SceneGate>());
	gates_.back()->Initialize("player","1-1");

	temp = ADXObject::Create({ 10,1.01f,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 });
	gates_.push_back(temp->AddComponent<SceneGate>());
	gates_.back()->Initialize("player", "1-2");

	temp = ADXObject::Create({ 20,1.01f,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 });
	gates_.push_back(temp->AddComponent<SceneGate>());
	gates_.back()->Initialize("player", "1-BOSS");


	backGround_ = ADXObject::Create();
	backGround_->transform_.rectTransform_ = true;
	backGround_->transform_.UpdateMatrix();
	backGround_->GetComponent<ADXModelRenderer>()->model_ = &rect_;
	backGround_->GetComponent<ADXModelRenderer>()->texture_ = backGroundTex_;
	backGround_->renderLayer_ = -1;

	key_ = ADXObject::Duplicate(*backGround_);
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localScale_ = { 0.3f,0.45f,1 };
	key_->transform_.UpdateMatrix();
	key_->GetComponent<ADXModelRenderer>()->texture_ = keyImg_;
	key_->renderLayer_ = 1;
}

void StageSelectScene::Update()
{
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localPosition_.y_ += sinf(clock() * 0.001f) * 0.01f;
	key_->transform_.localScale_ = { 0.45f / ADXWindow::GetInstance()->GetAspect(),0.45f,1 };

	if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("return"))
	{
		SceneTransition::ChangeScene(1);
	}
}