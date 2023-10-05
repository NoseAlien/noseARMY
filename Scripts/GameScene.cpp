#include "GameScene.h"
#include "SceneTransition.h"

#include "Cub_E.h"

void GameScene::Initialize()
{
	floors_ = {};
	fields_ = {};
	battleFields_ = {};
	enemies_ = {};
	tutorialAreas_ = {};
	player_ = {};

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
	floors_.back()->transform.localScale_ = { 10,1,10 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 0,-1,15 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 10,1,5 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 0,10,25 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 10,10,5 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 0,9,40 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 10,10,10 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 9.5,19.5,40 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 0.5,0.5,10 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { -9.5,19.5,40 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 0.5,0.5,10 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 0,19,60 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 10,1,10 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 0,18,77 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.5,0,0 });
	floors_.back()->transform.localScale_ = { 10,9,1 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 4,10,61 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 14,1,11 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 13.5f,10,93 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 4.5f,1,21 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 10,11.5f,85 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 1,0.5f,11 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { 3,10,112 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 6,1,6 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { -18.5f,-14.8f,112 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,1 });
	floors_.back()->transform.localScale_ = { 30,1,2 };
	floors_.back()->transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(*floors_.back()));
	floors_.back()->transform.localPosition_ = { -40,-60,112 };
	floors_.back()->transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back()->transform.localScale_ = { 6,1,6 };
	floors_.back()->transform.UpdateMatrix();


	temp = ADXObject::Create({ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("tutorial_move.png"));

	temp = ADXObject::Create({ 0,0,8 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,2 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("tutorial_jump.png"));

	temp = ADXObject::Create({ 0,10,15 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,15 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("tutorial_jump_2.png"));

	temp = ADXObject::Create({ 0,23,38 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,4,15 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("tutorial_army.png"));

	temp = ADXObject::Create({ 3,15.5,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,5,6 });
	tutorialAreas_.push_back(temp->AddComponent<TutorialArea>());
	tutorialAreas_.back()->SetTutorialImg(ADXImage::LoadADXImage("tutorial_army_2.png"));


	temp = ADXObject::Create({ 0,13,35 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,14,45 });
	fields_.push_back(temp->AddComponent<FieldBox>());

	temp = ADXObject::Create({ 13.5,15,82 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4.5,5,32 });
	fields_.push_back(temp->AddComponent<FieldBox>());

	temp = ADXObject::Create({ 3,15.5f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6.1f,5,6 });
	fields_.push_back(temp->AddComponent<FieldBox>());

	temp = ADXObject::Create({ -22,-14,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,1 }), { 38,4,2 });
	fields_.push_back(temp->AddComponent<FieldBox>());

	temp = ADXObject::Create({ -40,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 });
	fields_.push_back(temp->AddComponent<FieldBox>());


	temp = ADXObject::Create({ 3,15.4f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,5,6 });
	battleFields_.push_back(temp->AddComponent<BattleFieldBox>());
	battleFields_.back()->Initialize({
		{1,{0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
		{2,{0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
		{1,{-0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
		{2,{-0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
		}, "enemy");
	battleFields_.back()->fieldLayer = 2;

	enemySpawnData.SetSpawnList({
		{ 2,{2,24,40},ADXQuaternion::IdentityQuaternion() },
		{ 1,{-2,24,40},ADXQuaternion::IdentityQuaternion() },
		});

	enemySpawnData.Spawn("enemy");

	temp = ADXObject::Create({ -40,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 });
	goal_ = temp->AddComponent<Goal>();
	goal_->Initialize("player");

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

void GameScene::Update()
{
	key->transform.localPosition_ = { -0.65f,0.85f,0 };
	key->transform.localPosition_.y += sinf(clock() * 0.001f) * 0.01f;
	key->transform.localScale_ = { 0.45f / ADXWindow::GetAspect(),0.45f,1 };

	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_Q))
	{
		SceneTransition::ChangeScene(2);
	}
}