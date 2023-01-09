#include "ADXScene.h"

ADXScene::ADXScene()
{

}

void ADXScene::Initialize(ADXKeyBoardInput* setKeyboard, ID3D12Device* setDevice)
{
	keyboard = setKeyboard;
	device = setDevice;

	//画像
	skyDomeImg = ADXImage::LoadADXImage("skydome/Fine_Basin.jpg");
	MEGNOSE_Img = ADXImage::LoadADXImage("MEGNOSE.png");
	groundImg = ADXImage::LoadADXImage("GroundBlock.png");
	battleFieldImg = ADXImage::LoadADXImage("battleField.png");

	//マテリアル
	material = ADXMaterial::LoadMaterial("material/shade.mtl");
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect.vertices = {
		{{-1.0f,-1.0f,0.0f},{}, {0.0f,1.0f}},//左下
		{{-1.0f,1.0f,0.0f},{},{0.0f,0.0f}},//左上
		{{1.0f,-1.0f,0.0f},{},{1.0f,1.0f}},//右下
		{{1.0f,1.0f,0.0f},{},{1.0f,0.0f}},//右上
	};
	//インデックスデータ
	rect.indices =
	{
		0,1,2,
		2,1,3,

		1,0,2,
		1,2,3,
	};
	rect.Initialize();

	cube = ADXModel::LoadModel("model/cube.obj");
	ground = ADXModel::LoadModel("model/groundBlock.obj");
	playerModel = ADXModel::LoadModel("model/sphere.obj");
	skyDomeModel = ADXModel::LoadModel("skydome/skydome.obj");

	//オブジェクト

	camera_.ADXObject::Initialize();
	camera_.transform.translation_ = { 0,5,-20 };
	camera_.transform.rotation_ = { 0.3,0,0 };
	camera_.Initialize();

	player_.ADXObject::Initialize();
	player_.transform.translation_ = { 0,2,0 };
	player_.transform.rotation_ = { 0,0,0 };
	player_.transform.scale_ = { 0.5,0.5,0.5 };
	player_.transform.UpdateMatrix();
	player_.model = &playerModel;
	player_.colliders.push_back(ADXCollider(&player_));
	player_.colliders.back().pushable_ = true;
	player_.material = unlitMat;
	player_.Initialize(keyboard, { DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE,DIK_C },&camera_);

	floors_.push_back(ADXObject());
	floors_.back().Initialize();
	floors_.back().transform.translation_ = { 0,-1,0 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 2,2,2 };
	floors_.back().transform.UpdateMatrix();
	floors_.back().model = &ground;
	floors_.back().texture = groundImg;
	floors_.back().colliders.push_back(ADXCollider(&floors_.back()));
	floors_.back().colliders.back().colType_ = box;
	floors_.back().material = unlitMat;

	floors_.push_back(ADXObject::Duplicate(floors_.back(),true));
	floors_.back().transform.translation_ = { 0,-2,0 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 10,1,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 0,-1,15 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 10,1,5 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 0,10,25 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 10,10,5 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 0,9,40 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 10,10,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 9.5,19.5,40 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 0.5,0.5,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { -9.5,19.5,40 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 0.5,0.5,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 0,19,60 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 10,1,10 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 0,18,77 };
	floors_.back().transform.rotation_ = { 0.5,0,0 };
	floors_.back().transform.scale_ = { 10,9,1 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 4,10,61 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 14,1,11 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 13.5,10,93 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 4.5,1,21 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 10,11.5,85 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 1,0.5,11 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 3,10.75,112 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 6,0.25,2 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { 3,9.5,112 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 6,1,6 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { -18.5,-14.8,112 };
	floors_.back().transform.rotation_ = { 0,0,1 };
	floors_.back().transform.scale_ = { 30,1,2 };
	floors_.back().transform.UpdateMatrix();

	floors_.push_back(ADXObject::Duplicate(floors_.back(), true));
	floors_.back().transform.translation_ = { -40,-60,112 };
	floors_.back().transform.rotation_ = { 0,0,0 };
	floors_.back().transform.scale_ = { 6,1,6 };
	floors_.back().transform.UpdateMatrix();

	TutorialArea newArea;
	ADXObject* newAreaObj;

	tutorialAreas_.push_back(TutorialArea());
	tutorialAreas_.back().ADXObject::Initialize();
	tutorialAreas_.back().transform.translation_ = { 0,3,0 };
	tutorialAreas_.back().transform.rotation_ = { 0,0,0 };
	tutorialAreas_.back().transform.scale_ = { 4,4,4 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().model = &ground;
	tutorialAreas_.back().texture = battleFieldImg;
	tutorialAreas_.back().colliders.push_back(ADXCollider(&tutorialAreas_.back()));
	tutorialAreas_.back().colliders.back().isTrigger = true;
	tutorialAreas_.back().colliders.back().colType_ = box;
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_move.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.translation_ = { 0,0,8 };
	tutorialAreas_.back().transform.rotation_ = { 0,0,0 };
	tutorialAreas_.back().transform.scale_ = { 10,1,2 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_jump.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.translation_ = { 0,10,15 };
	tutorialAreas_.back().transform.rotation_ = { 0,0,0 };
	tutorialAreas_.back().transform.scale_ = { 10,10,5 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_jump_2.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.translation_ = { 0,22,38 };
	tutorialAreas_.back().transform.rotation_ = { 0,0,0 };
	tutorialAreas_.back().transform.scale_ = { 10,3,15 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_army.png"));

	newAreaObj = &newArea;
	*newAreaObj = ADXObject::Duplicate(tutorialAreas_.back(), true);
	tutorialAreas_.push_back(newArea);
	tutorialAreas_.back().transform.translation_ = { 3,15.5,112 };
	tutorialAreas_.back().transform.rotation_ = { 0,0,0 };
	tutorialAreas_.back().transform.scale_ = { 6,5,6 };
	tutorialAreas_.back().transform.UpdateMatrix();
	tutorialAreas_.back().Initialize(ADXImage::LoadADXImage("tutorial_army_2.png"));


	backGround_.Initialize();
	backGround_.transform.rectTransform = true;
	backGround_.transform.UpdateMatrix();
	backGround_.model = &rect;
	backGround_.texture = MEGNOSE_Img;
	backGround_.material = unlitMat;
	backGround_.renderLayer = -1;

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
	objs.push_back(&backGround_);
}

void ADXScene::Update()
{
	for (auto& itr : objs)
	{
		itr->Update();
	}
}