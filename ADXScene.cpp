#include "ADXScene.h"

ADXScene::ADXScene()
{

}

void ADXScene::Initialize(ADXKeyBoardInput* setKeyboard, ID3D12Device* setDevice, const int* set_window_width, const int* set_window_height)
{
	keyboard = setKeyboard;
	device = setDevice;
	window_width = set_window_width;
	window_height = set_window_height;

	//射影変換行列（透視投影）
	matProjection = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(45.0f),//画角
			(float)*window_width / *window_height,//アスペクト比
			0.1f, 1000.0f));//ニア、ファークリップ

	//ビュー変換行列
	eye = XMFLOAT3(0, 0, -20);
	target = XMFLOAT3(0, 0, 0);
	up = XMFLOAT3(0, 1, 0);
	matView = ADXMatrix4::ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));

	ADXWorldTransform::StaticInitialize(&matView, &matProjection);

	//画像
	napnoseImg = ADXImage::LoadADXImage("napnose.png");
	MEGNOSE_Img = ADXImage::LoadADXImage("MEGNOSE.png");
	skyDomeImg = ADXImage::LoadADXImage("skydome/Fine_Basin.jpg");
	whiteDotImg = ADXImage::LoadADXImage("whiteDot.png");

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
	playerModel = ADXModel::LoadModel("model/sphere.obj");
	skyDomeModel = ADXModel::LoadModel("skydome/skydome.obj");

	//オブジェクト

	player_.Initialize();
	player_.transform.translation_ = { 0,2,0 };
	player_.transform.rotation_ = { 0,0,0 };
	player_.transform.scale_ = { 0.5,0.5,0.5 };
	player_.transform.UpdateMatrix();
	player_.model = &playerModel;
	player_.texture = whiteDotImg;
	player_.colliders.push_back(ADXCollider(&player_));
	//player_.colliders.back().colType_ = box;
	player_.colliders.back().pushable_ = true;
	player_.players.push_back(ADXPlayer(&player_, keyboard, { DIK_UP,DIK_DOWN,DIK_RIGHT,DIK_LEFT,DIK_SPACE }));
	player_.material = material;

	playerMarker_.Initialize();
	playerMarker_.transform.translation_ = { 0,1.6,0 };
	playerMarker_.transform.rotation_ = { 0,0,0 };
	playerMarker_.transform.scale_ = { 0.6,0.5,0.5 };
	playerMarker_.transform.parent_ = &player_.transform;
	playerMarker_.transform.UpdateMatrix();
	playerMarker_.model = &rect;
	playerMarker_.texture = napnoseImg;
	playerMarker_.material = unlitMat;
	playerMarker_.renderLayer = 1;

	object_.Initialize();
	object_.transform.translation_ = { 0,-2,0 };
	object_.transform.rotation_ = { 0,-0.4,0 };
	object_.transform.scale_ = { 8,0.3,8 };
	object_.transform.UpdateMatrix();
	object_.model = &cube;
	object_.texture = whiteDotImg;
	object_.colliders.push_back(ADXCollider(&object_));
	object_.colliders.back().colType_ = box;
	//object_.colliders.back().pushable_ = true;
	object_.material = lightShadeMat;

	object2_.Initialize();
	object2_.transform.translation_ = { 0,-2,0 };
	object2_.transform.rotation_ = { 0,0.4,0 };
	object2_.transform.scale_ = { 8,0.3,8 };
	object2_.transform.UpdateMatrix();
	object2_.model = &cube;
	object2_.texture = whiteDotImg;
	object2_.colliders.push_back(ADXCollider(&object2_));
	object2_.colliders.back().colType_ = box;
	//object2_.colliders.back().pushable_ = true;
	object2_.material = lightShadeMat;

	object3_.Initialize();
	object3_.transform.translation_ = { 0,-1,0 };
	object3_.transform.rotation_ = { 0,0,0.3 };
	object3_.transform.scale_ = { 2,2,2 };
	object3_.transform.UpdateMatrix();
	object3_.model = &cube;
	object3_.texture = whiteDotImg;
	object3_.colliders.push_back(ADXCollider(&object2_));
	object3_.colliders.back().colType_ = box;
	//object3_.colliders.back().pushable_ = true;
	object3_.material = lightShadeMat;

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

	objs.push_back(&player_);
	objs.push_back(&playerMarker_);
	objs.push_back(&object_);
	objs.push_back(&object2_);
	objs.push_back(&object3_);
	//objs.push_back(&skyDome_);
	objs.push_back(&sprite_);
}

void ADXScene::Update()
{
	for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->Update();
	}

	matView = matView.ConvertToADXMatrix(
		XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up)));
}