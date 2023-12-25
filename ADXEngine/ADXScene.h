#pragma once
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "ADXImage.h"
#include "ADXAudio.h"
#include "ADXKeyBoardInput.h"
#include "ADXGamePadInput.h"

class ADXScene
{
private:
	//�摜
	uint32_t engineLogoImg_ = 0;
	uint32_t logoImg_A_ = 0;
	uint32_t logoImg_D_ = 0;
	uint32_t logoImg_X_ = 0;
	uint32_t logoImg_Engine_ = 0;
	uint32_t titleImg_ = 0;

	//�����f�[�^
	ADXAudio logoJingle_{};

	//3D���f��
	ADXModel rect_{};

	//�}�e���A��
	ADXMaterial unlitMat_{};

	//�J����
	ADXCamera* camera_ = nullptr;

	//�I�u�W�F�N�g
	ADXObject* engineLogo_ = nullptr;
	ADXObject* logo_A_ = nullptr;
	ADXObject* logo_D_ = nullptr;
	ADXObject* logo_X_ = nullptr;
	ADXObject* logo_Engine_ = nullptr;
	ADXObject* title_ = nullptr;

	int32_t startTime_ = 0;

public:
	ADXScene();
	virtual void Initialize();
	virtual void Update();
};