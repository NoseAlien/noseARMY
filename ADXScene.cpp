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

	//マテリアル
	material = ADXMaterial::LoadMaterial("material/shade.mtl");
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");
	lightShadeMat = ADXMaterial::LoadMaterial("material/lightShade.mtl");

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
	player_.Initialize(keyboard, { DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE,DIK_C });

	object_.Initialize();
	object_.transform.translation_ = { 0,-2,0 };
	object_.transform.rotation_ = { 0,-0.4,0 };
	object_.transform.scale_ = { 8,0.3,8 };
	object_.transform.UpdateMatrix();
	object_.model = &ground;
	object_.texture = groundImg;
	object_.colliders.push_back(ADXCollider(&object_));
	object_.colliders.back().colType_ = box;
	object_.material = lightShadeMat;

	object2_ = ADXObject::Duplicate(object_);
	object2_.transform.translation_ = { 0,-2,0 };
	object2_.transform.rotation_ = { 0,0.4,0 };
	object2_.transform.scale_ = { 8,0.3,8 };

	object3_ = ADXObject::Duplicate(object_);
	object3_.transform.translation_ = { 0,-1,0 };
	object3_.transform.rotation_ = { 0,0,0.3 };
	object3_.transform.scale_ = { 2,2,2 };

	skyDome_.Initialize();
	skyDome_.model = &skyDomeModel;
	skyDome_.texture = skyDomeImg;
	skyDome_.material = material;

	sprite_.Initialize();
	sprite_.transform.rectTransform = true;
	sprite_.transform.UpdateMatrix();
	sprite_.model = &rect;
	sprite_.texture = MEGNOSE_Img;
	sprite_.material = unlitMat;
	sprite_.renderLayer = -1;

	objs.push_back(&camera_);
	objs.push_back(&player_);
	objs.push_back(&object_);
	objs.push_back(&object2_);
	objs.push_back(&object3_);
	objs.push_back(&sprite_);
}

void ADXScene::Update()
{
	for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->Update();
	}
}