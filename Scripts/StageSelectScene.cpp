#include "StageSelectScene.h"
#include "SceneTransition.h"

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


	temp = ADXObject::Create({ 0,5,-20 }, ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 }));
	camera_ = temp->AddComponent<ADXCamera>();

	temp = ADXObject::Create({ 0,2,0 });
	player_ = temp->AddComponent<Player>();
	player_->Initialize(ADXKeyBoardInput::GetCurrentInstance(),
		{ DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE,DIK_C },
		ADXGamePadInput::GetCurrentInstance(),
		{ A,B },
		camera_);
	player_->LiveEntity::Initialize("player");

	floors_.push_back(ADXObject::Create({ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 }));
	floors_.back()->model_ = &ground_;
	floors_.back()->texture_ = groundImg_;
	ADXCollider* tempCol = floors_.back()->AddComponent<ADXCollider>();
	tempCol->colType_ = box;

	floors_.push_back(ADXObject::Create({ 0,-2,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 50,1,50 }));
	floors_.back()->model_ = &ground_;
	floors_.back()->texture_ = groundImg_;
	tempCol = floors_.back()->AddComponent<ADXCollider>();
	tempCol->colType_ = box;


	temp = ADXObject::Create({ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_move.png"));

	temp = ADXObject::Create({ 0,13,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 50,14,50 });
	fields_.push_back(temp->AddComponent<FieldBox>());


	temp = ADXObject::Create({ 0,1.01f,30 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 });
	gates_.push_back(temp->AddComponent<SceneGate>());
	gates_.back()->Initialize("player");


	backGround_ = ADXObject::Create();
	backGround_->transform_.rectTransform_ = true;
	backGround_->transform_.UpdateMatrix();
	backGround_->model_ = &rect_;
	backGround_->texture_ = backGroundTex_;
	backGround_->renderLayer_ = -1;

	key_ = ADXObject::Duplicate(*backGround_);
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localScale_ = { 0.3f,0.45f,1 };
	key_->transform_.UpdateMatrix();
	key_->texture_ = keyImg_;
	key_->renderLayer_ = 1;
}

void StageSelectScene::Update()
{
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localPosition_.y_ += sinf(clock() * 0.001f) * 0.01f;
	key_->transform_.localScale_ = { 0.45f / ADXWindow::GetAspect(),0.45f,1 };

	if (ADXKeyBoardInput::GetCurrentInstance()->GetKeyDown(DIK_Q))
	{
		SceneTransition::ChangeScene(1);
	}
}