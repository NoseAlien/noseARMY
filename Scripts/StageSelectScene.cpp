#include "StageSelectScene.h"
#include "SceneTransition.h"

StageSelectScene::StageSelectScene()
{
}

void StageSelectScene::Initialize()
{
	objs = {};
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

	camera_.ADXObject::Initialize();
	camera_.transform.localPosition_ = { 0,5,-20 };
	camera_.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0.3f,0,0 });
	camera_.Initialize();

	player_ = {};
	player_.ADXObject::Initialize();
	player_.transform.localPosition_ = { 0,2,0 };
	player_.transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	player_.transform.localScale_ = { 0.5f,0.5f,0.5f };
	player_.transform.UpdateMatrix();
	player_.Initialize(ADXKeyBoardInput::GetCurrentInstance(), { DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE,DIK_C }, &camera_);
	player_.LiveEntity::Initialize("player");

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
	floors_.back().transform.localScale_ = { 50,1,50 };
	floors_.back().transform.UpdateMatrix();


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


	fields_.push_back(FieldBox());
	fields_.back().ADXObject::Initialize();
	fields_.back().transform.localPosition_ = { 0,13,0 };
	fields_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	fields_.back().transform.localScale_ = { 50,14,50 };
	fields_.back().transform.UpdateMatrix();
	fields_.back().colliders.push_back(ADXCollider(&fields_.back()));
	fields_.back().colliders.back().isTrigger = true;
	fields_.back().colliders.back().colType_ = box;


	gates_.push_back(SceneGate());
	gates_.back().ADXObject::Initialize();
	gates_.back().transform.localPosition_ = { 0,1.01f,30 };
	gates_.back().transform.localRotation_ = ADXQuaternion::EulerToQuaternion({ 0,0,0 });
	gates_.back().transform.localScale_ = { 2,2,2 };
	gates_.back().transform.UpdateMatrix();
	gates_.back().Initialize("player");


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
	for (auto& itr : gates_)
	{
		objs.push_back(&itr);
	}
	objs.push_back(&backGround_);
	objs.push_back(&key);
}

void StageSelectScene::Update()
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
		SceneTransition::ChangeScene(1);
	}
}