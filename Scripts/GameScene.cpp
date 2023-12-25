#include "GameScene.h"
#include "SceneTransition.h"

#include "ADXKeyConfig.h"

void GameScene::Initialize()
{
	floors_ = {};
	fields_ = {};
	battleFields_ = {};
	player_ = {};

	//画像
	keyImg_ = ADXImage::LoadADXImage("texture/QUIT_TITLE.png");
	backGroundTex_ = ADXImage::LoadADXImage("texture/skyBG.png");
	groundImg_ = ADXImage::LoadADXImage("texture/GroundBlock.png");

	rect_ = ADXModel::CreateRect();
	ground_ = ADXModel::LoadADXModel("model/groundBlock.obj");

	//オブジェクト

	ADXObject* temp = ADXObject::Create();
	shutter_ = temp->AddComponent<SceneTransition>();

	temp = ADXObject::Create({ 0,5,-20 }, ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 }));

	temp->transform_.localPosition_ = { 0,5,-20 };
	temp->transform_.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 });
	camera_ = temp->AddComponent<ADXCamera>();

	temp = ADXObject::Create({ 0,2,0 });
	player_ = temp->AddComponent<Player>();
	player_->Initialize(camera_);
	player_->LiveEntity::Initialize("player");


	floorGenerateData_ = {
		{{ 0,-1,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 2,2,2 }},
		{{ 0,-2,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }},
		{{ 0,-1,15 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,5 }},
		{{ 0,10,25 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,5 }},
		{{ 0,9,40 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,10 }},
		{{ 9.5f,19.5f,40 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.5f,0.5f,10 }},
		{{ -9.5f,19.5f,40 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 0.5f,0.5f,10 }},
		{{ 0,19,60 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 }},
		{{ 0,18,77 }, ADXQuaternion::EulerToQuaternion({ 0.5f,0,0 }), { 10,9,1 }},
		{{ 4,10,61 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 14,1,11 }},
		{{ 13.5f,10,93 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4.5f,1,21 }},
		{{ 10,11.5f,85 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 1,0.5f,11 }},
		{{ 3,10,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,1,6 }},
		{{ -18.5f,-14.8f,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,1 }), { 30,1,2 }},
		{{ -40,-60,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,1,6 }},
	};

	for (auto& itr : floorGenerateData_)
	{
		floors_.push_back(ADXObject::Create(itr.localPosition,itr.localRotation, itr.localScale));
		floors_.back()->model_ = &ground_;
		floors_.back()->texture_ = groundImg_;
		ADXCollider* tempCol = floors_.back()->AddComponent<ADXCollider>();
		tempCol->colType_ = box;
	}


	temp = ADXObject::Create({ 0,3,0 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 4,4,4 });
	TutorialArea* tempTutorialArea = temp->AddComponent<TutorialArea>();
	tempTutorialArea->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_move.png"));

	temp = ADXObject::Create({ 0,-1.5f,16 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,1,10 });
	tempTutorialArea = temp->AddComponent<TutorialArea>();
	tempTutorialArea->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_jump.png"));

	temp = ADXObject::Create({ 0,10,15.5f }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,4 });
	tempTutorialArea = temp->AddComponent<TutorialArea>();
	tempTutorialArea->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_jump_2.png"));

	temp = ADXObject::Create({ 0,23,38 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,4,15 });
	tempTutorialArea = temp->AddComponent<TutorialArea>();
	tempTutorialArea->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_army.png"));

	temp = ADXObject::Create({ 3,15.5,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,5,6 });
	tempTutorialArea = temp->AddComponent<TutorialArea>();
	tempTutorialArea->SetTutorialImg(ADXImage::LoadADXImage("texture/tutorial_army_2.png"));


	temp = ADXObject::Create({ 0,13,5 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,14,15 });
	fields_.push_back(temp->AddComponent<FieldBox>());

	temp = ADXObject::Create({ 0,18,35 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 10,10,35 });
	fields_.push_back(temp->AddComponent<FieldBox>());

	temp = ADXObject::Create({ 0,18,66 }, ADXQuaternion::EulerToQuaternion({ 0.5f,0,0 }), { 10,9,10 });
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
	battleFields_.back()->fieldLayer_ = 2;

	enemySpawnData_.SetSpawnList({
		{ 1,{2,20,40},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
		{ 1,{-2,20,44},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
		{ 2,{0,21,55},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
		{ 1,{15,12,86},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },
		{ 1,{14,12,100},ADXQuaternion::EulerToQuaternion({0,ADXUtility::Pi,0}) },


		});

	enemySpawnData_.Spawn("enemy");

	temp = ADXObject::Create({ -40,-50,112 }, ADXQuaternion::EulerToQuaternion({ 0,0,0 }), { 6,10,6 });
	goal_ = temp->AddComponent<Goal>();
	goal_->Initialize("player");

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

void GameScene::Update()
{
	key_->transform_.localPosition_ = { -0.65f,0.85f,0 };
	key_->transform_.localPosition_.y_ += sinf(clock() * 0.001f) * 0.01f;
	key_->transform_.localScale_ = { 0.45f / ADXWindow::GetAspect(),0.45f,1 };

	if (ADXKeyConfig::GetCurrentInstance()->GetInputDown("return"))
	{
		SceneTransition::ChangeScene(2);
	}
}