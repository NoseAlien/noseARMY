#include "GameScene.h"
#include "SceneTransition.h"

GameScene::GameScene()
{
}

void GameScene::Initialize()
{
	objs = {};
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
	ground = ADXModel::LoadModel("model/groundBlock.obj");

	//オブジェクト

	camera_.ADXObject::Initialize();
	camera_.transform.localPosition_ = { 0,5,-20 };
	camera_.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 });
	camera_.Initialize();

	player_.ADXObject::Initialize();
	player_.transform.localPosition_ = { 0,2,0 };
	player_.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	player_.transform.localScale_ = { 0.5f,0.5f,0.5f };
	player_.transform.UpdateMatrix();
	player_.Initialize(ADXKeyBoardInput::GetCurrentInstance(), { DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE,DIK_C }, &camera_);
	player_.Species::Initialize("player");

	floors_.push_back(ADXObject());
	floors_.back().Initialize();
	floors_.back().transform.localPosition_ = { 0,-1,0 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 2,2,2 };
	floors_.back().transform.UpdateMatrix();
	floors_.back().model = &ground;
	floors_.back().texture = groundImg;
	floors_.back().colliders.push_back(ADXCollider(&floors_.back()));
	floors_.back().colliders.back().colType_ = box;

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 0,-2,0 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 10,1,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 0,-1,15 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 10,1,5 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 0,10,25 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 10,10,5 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 0,9,40 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 10,10,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 9.5,19.5,40 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 0.5,0.5,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { -9.5,19.5,40 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 0.5,0.5,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 0,19,60 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 10,1,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 0,18,77 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.5,0,0 });
	floors_.back().transform.localScale_ = { 10,9,1 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 4,10,61 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 14,1,11 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 13.5f,10,93 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 4.5f,1,21 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 10,11.5f,85 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 1,0.5f,11 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 3,10.75f,112 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 6,0.25,2 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { 3,9.5f,112 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 6,1,6 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { -18.5f,-14.8f,112 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,1 });
	floors_.back().transform.localScale_ = { 30,1,2 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.localPosition_ = { -40,-60,112 };
	floors_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	floors_.back().transform.localScale_ = { 6,1,6 };
	floors_.back().transform.UpdateMatrix();


	TutorialArea newArea;
	ADXObject* newAreaObj;

	tutorialAreas_.push_back(TutorialArea());
	tutorialAreas_.back().ADXObject::Initialize();
	tutorialAreas_.back().transform.localPosition_ = { 0,3,0 };
	tutorialAreas_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	tutorialAreas_.back().transform.localScale_ = { 4,4,4 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().colliders.push_back(ADXCollider(&tutorialAreas_.back()));
	tutorialAreas_.back().colliders.back().isTrigger = true;
	tutorialAreas_.back().colliders.back().colType_ = box;
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_move.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.localPosition_ = { 0,0,8 };
	tutorialAreas_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	tutorialAreas_.back().transform.localScale_ = { 10,1,2 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_jump.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.localPosition_ = { 0,10,15 };
	tutorialAreas_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	tutorialAreas_.back().transform.localScale_ = { 10,10,5 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_jump_2.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.localPosition_ = { 0,23,38 };
	tutorialAreas_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	tutorialAreas_.back().transform.localScale_ = { 10,4,15 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_army.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.localPosition_ = { 3,15.5,112 };
	tutorialAreas_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	tutorialAreas_.back().transform.localScale_ = { 6,5,6 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_army_2.png"));


	FieldBox newField;
	ADXObject* newFieldObj;

	fields_.push_back(FieldBox());
	fields_.back().ADXObject::Initialize();
	fields_.back().transform.localPosition_ = { 0,13,35 };
	fields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	fields_.back().transform.localScale_ = { 10,14,45 };
	fields_.back().transform.UpdateMatrix();
	fields_.back().Initialize();

	newFieldObj = &newField;
	*newFieldObj = ADXObject::Duplicate(fields_.back(), true);
	fields_.push_back(newField);
	fields_.back().transform.localPosition_ = { 13.5,15,82 };
	fields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	fields_.back().transform.localScale_ = { 4.5,5,32 };
	fields_.back().transform.UpdateMatrix();

	newFieldObj = &newField;
	*newFieldObj = ADXObject::Duplicate(fields_.back(), true);
	fields_.push_back(newField);
	fields_.back().transform.localPosition_ = { 3,15.5f,112 };
	fields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	fields_.back().transform.localScale_ = { 6,5,6 };
	fields_.back().transform.UpdateMatrix();

	newFieldObj = &newField;
	*newFieldObj = ADXObject::Duplicate(fields_.back(), true);
	fields_.push_back(newField);
	fields_.back().transform.localPosition_ = { -22,-14,112 };
	fields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,1 });
	fields_.back().transform.localScale_ = { 38,4,2 };
	fields_.back().transform.UpdateMatrix();

	newFieldObj = &newField;
	*newFieldObj = ADXObject::Duplicate(fields_.back(), true);
	fields_.push_back(newField);
	fields_.back().transform.localPosition_ = { -40,-50,112 };
	fields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	fields_.back().transform.localScale_ = { 6,10,6 };
	fields_.back().transform.UpdateMatrix();


	battleFields_.push_back(BattleFieldBox());
	battleFields_.back().ADXObject::Initialize();
	battleFields_.back().transform.localPosition_ = { 3,15.4f,112 };
	battleFields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	battleFields_.back().transform.localScale_ = { 6,5,6 };
	battleFields_.back().transform.UpdateMatrix();
	battleFields_.back().Initialize({ 
		{{0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
		{{0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
		{{-0.5f,0,0.5f},ADXQuaternion::IdentityQuaternion()},
		{{-0.5f,0,-0.5f},ADXQuaternion::IdentityQuaternion()},
		}, "enemy");
	battleFields_.back().fieldLayer = 2;


	Enemy newEnemy;
	ADXObject* newEnemyObj;

	enemies_.push_back(Enemy());
	enemies_.back().ADXObject::Initialize();
	enemies_.back().transform.localPosition_ = { 0,40,40 };
	enemies_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	enemies_.back().transform.localScale_ = { 1,1,1 };
	enemies_.back().transform.UpdateMatrix();
	enemies_.back().Initialize();
	enemies_.back().Species::Initialize("enemy");

	newEnemyObj = &newEnemy;
	*newEnemyObj = ADXObject::Duplicate(enemies_.back(), true);
	enemies_.push_back(newEnemy);
	enemies_.back().transform.localPosition_ = { 0,41,40 };
	enemies_.back().transform.UpdateMatrix();
	enemies_.back().Initialize();
	enemies_.back().Species::Initialize("enemy");


	goal_.ADXObject::Initialize();
	goal_.transform.localPosition_ = { -40,-50,112 };
	goal_.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	goal_.transform.localScale_ = { 6,10,6 };
	goal_.transform.UpdateMatrix();
	goal_.Initialize("player");

	backGround_.Initialize();
	backGround_.transform.rectTransform = true;
	backGround_.transform.UpdateMatrix();
	backGround_.model = &rect;
	backGround_.texture = backGroundTex;
	backGround_.renderLayer = -1;

	key = ADXObject::Duplicate(backGround_);
	key.transform.localPosition_ = { -0.65f,0.85f,0 };
	key.transform.localScale_ = { 0.3f,0.45f,1 };
	key.transform.UpdateMatrix();
	key.texture = keyImg;
	key.renderLayer = 1;

	objs.push_back(&camera_);
	objs.push_back(&player_);
	for (auto& itr : floors_)
	{
		objs.push_back(&itr);
	}
	for (auto& itr : tutorialAreas_)
	{
		objs.push_back(&itr);
	}
	for (auto& itr : fields_)
	{
		objs.push_back(&itr);
	}
	for (auto& itr : battleFields_)
	{
		objs.push_back(&itr);
	}
	for (auto& itr : enemies_)
	{
		objs.push_back(&itr);
	}
	objs.push_back(&goal_);
	objs.push_back(&backGround_);
	objs.push_back(&key);
}

void GameScene::Update()
{
	key.transform.localPosition_ = { -0.65f,0.85f,0 };
	key.transform.localPosition_.y += sinf(clock() * 0.001f) * 0.01f;
	key.transform.localScale_ = { 0.45f / ADXWindow::GetAspect(),0.45f,1 };

	for (auto& itr : objs)
	{
		itr->Update();
	}

	if (ADXKeyBoardInput::GetCurrentInstance()->KeyTrigger(DIK_Q))
	{
		SceneTransition::ChangeScene(2);
	}
}