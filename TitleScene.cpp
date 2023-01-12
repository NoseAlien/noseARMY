#include "TitleScene.h"

TitleScene::TitleScene()
{
}

void TitleScene::Initialize()
{
	
	//�摜
	titleImg = ADXImage::LoadADXImage("noseARMY_logo.png");
	keyImg = ADXImage::LoadADXImage("PRESS_SPACE.png");

	//�}�e���A��
	unlitMat = ADXMaterial::LoadMaterial("material/unlit.mtl");

	rect.vertices = {
		{{-1.0f,-1.0f,0.0f},{}, {0.0f,1.0f}},//����
		{{-1.0f,1.0f,0.0f},{},{0.0f,0.0f}},//����
		{{1.0f,-1.0f,0.0f},{},{1.0f,1.0f}},//�E��
		{{1.0f,1.0f,0.0f},{},{1.0f,0.0f}},//�E��
	};
	//�C���f�b�N�X�f�[�^
	rect.indices =
	{
		0,1,2,
		2,1,3,

		1,0,2,
		1,2,3,
	};
	rect.Initialize();

	//�I�u�W�F�N�g

	camera_.ADXObject::Initialize();
	camera_.transform.localPosition_ = { 0,0,-2 };
	camera_.transform.localEulerAngles_ = { 0,0,0 };
	camera_.Initialize();

	title.Initialize();
	title.transform.UpdateMatrix();
	title.texture = titleImg;
	title.model = &rect;
	title.material = unlitMat;

	key = ADXObject::Duplicate(title);
	key.transform.localScale_ = { 0.5,0.5,0.5 };
	key.texture = keyImg;


	objs.push_back(&camera_);
	objs.push_back(&title);
	objs.push_back(&key);
}

void TitleScene::Update()
{
	title.transform.localPosition_.y = sin(clock() * 0.001) * 0.01 + 0.2;
	key.transform.localPosition_.y = sin(clock() * 0.001 - 1) * 0.01 - 0.4;

	for (auto& itr : objs)
	{
		itr->Update();
	}
}