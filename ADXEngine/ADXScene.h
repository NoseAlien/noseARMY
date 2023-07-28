#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "ADXImage.h"
#include "ADXAudio.h"

class ADXScene
{
private:
	//�摜
	uint32_t engineLogoImg = 0;
	uint32_t logoImg_A = 0;
	uint32_t logoImg_D = 0;
	uint32_t logoImg_X = 0;
	uint32_t logoImg_Engine = 0;
	uint32_t titleImg = 0;
	uint32_t keyImg = 0;

	//�����f�[�^
	ADXAudio logoJingle{};

	//3D���f��
	ADXModel rect{};

	//�}�e���A��
	ADXMaterial unlitMat{};

	//�J����
	ADXCamera camera_{};

	//�I�u�W�F�N�g
	ADXObject engineLogo{};
	ADXObject logo_A{};
	ADXObject logo_D{};
	ADXObject logo_X{};
	ADXObject logo_Engine{};
	ADXObject title{};
	ADXObject key{};

	int32_t startTime = 0;

	//�S�ẴI�u�W�F�N�g
	std::list<ADXObject*> objs{};

public:
	ADXScene();
	virtual void Initialize();
	virtual void Update();
};