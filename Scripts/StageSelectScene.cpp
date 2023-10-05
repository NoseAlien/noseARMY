#include "StageSelectScene.h"
#include "SceneTransition.h"

StageSelectScene::StageSelectScene()
{
}

void StageSelectScene::Initialize()
{
	player_ = nullptr;
	floors_ = {};
	fields_ = {};
	tutorialAreas_ = {};

	//画像
	keyImg = ADXImage::LoadADXImage("QUIT_TITLE.png");
	backGroundTex = ADXImage::LoadADXImage("skyBG.png");
	groundImg = ADXImage::LoadADXImage("GroundBlock.png");

	rect = ADXModel::CreateRect();
	ground = ADXModel::LoadADXModel("model/groundBlock.obj");

	//オブジェクト

	ADXObject* temp = ADXObject::Create({ 0,5,-20 }, ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 }));
	temp->transform.localPosition_ = { 0,5,-20 };
	temp->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 });
	camera_ = temp->AddComponent<ADXCamera>();

	temp = ADXObject::Create({ 0,2,0 });
	player_ = temp->AddComponent<Player>();
	player_->Initialize(ADXKeyBoardInput::GetCurrentInstance(), { DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE,DIK_C }, camera_);
	player_->LiveEntity::Initialize("player");

	floors_.push_back(ADXObject::Create({ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 }));
	floors_.back()->model = &ground;
	floors_.back()->texture = groundImg;
	ADXCollider* tempCol = floors_.back()->AddComponent<ADXCollider>();
	tempCol->colType_ = box;

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 0,-2,0 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 50,1,50 };


	temp = ADXObject::Create({ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tempCol = temp->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;
	tutorialAreas_.back()->Initialize(ADXImage::LoadADXImage("tutorial_move.png"));

	temp = ADXObject::Create({ 0,13,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 50,14,50 });
	fields_.push_back(temp->AddComponent<FieldBox>());
	tempCol = temp->AddComponent<ADXCollider>();
	tempCol->isTrigger = true;
	tempCol->colType_ = box;


	temp = ADXObject::Create({ 0,1.01f,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 });
	gates_.push_back(temp->AddComponent<SceneGate>());
	gates_.back()->Initialize("player");


	backGround_ = ADXObject::Create();
	backGround_->transform.rectTransform = true;
	backGround_->transform.UpdateMatrix();
	backGround_->model = &rect;
	backGround_->texture = backGroundTex;
	backGround_->renderLayer = -1;

	key = ADXObject::Duplicate(*backGround_);
	key->transform.localPosition_ = { -0.65f,0.85f,0 };
	key->transform.localScale_ = { 0.3f,0.45f,1 };
	key->transform.UpdateMatrix();
	key->texture = keyImg;
	key->renderLayer = 1;
}

void StageSelectScene::Update()
{
	key->transform.localPosition_ = { -0.65f,0.85f,0 };
	key->transform.localPosition_.y += sinf(clock() * 0.001f) * 0.01f;
	key->transform.localScale_ = { 0.45f / ADXWindow::GetAspect(),0.45f,1 };

	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_Q))
	{
		SceneTransition::ChangeScene(1);
	}
}