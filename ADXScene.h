#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXWorldTransform.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "FieldBox.h"
#include "ADXImage.h"
#include "ADXTexAnimation.h"

class ADXScene
{
public:
	ADXKeyBoardInput* keyboard = nullptr;
	ID3D12Device* device = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE* srvHandle = nullptr;
	UINT64* CpuStartHandle = nullptr;
	UINT* incrementSize = nullptr;

	//�摜
	ADXImage MEGNOSE_Img{};
	ADXImage apEGnoSE_Image{};
	ADXImage skyDomeImg{};
	ADXImage groundImg{};
	ADXImage battleFieldImg{};

	//3D���f��
	ADXModel cube{};
	ADXModel ground{};
	ADXModel battleBox{};
	ADXModel rect{};
	ADXModel playerModel{};
	ADXModel skyDomeModel{};

	//�}�e���A��
	ADXMaterial material{};
	ADXMaterial unlitMat{};

	//�J����
	ADXCamera camera_{};

	//�I�u�W�F�N�g
	Player player_{};
	std::list<ADXObject> floors_{};
	std::list<TutorialArea> tutorialAreas_{};
	std::list<FieldBox> fields_{};

	ADXObject backGround_{};

public:
	ADXScene();
	void Initialize(ADXKeyBoardInput* setKeyboard, ID3D12Device* setDevice);
	void Update();

private:
	//�S�ẴI�u�W�F�N�g
	std::list<ADXObject*> objs{};
};