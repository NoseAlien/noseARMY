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

	//画像
	ADXImage MEGNOSE_Img{};
	ADXImage apEGnoSE_Image{};
	ADXImage skyDomeImg{};
	ADXImage groundImg{};
	ADXImage battleFieldImg{};

	//3Dモデル
	ADXModel cube{};
	ADXModel ground{};
	ADXModel battleBox{};
	ADXModel rect{};
	ADXModel playerModel{};
	ADXModel skyDomeModel{};

	//マテリアル
	ADXMaterial material{};
	ADXMaterial unlitMat{};

	//カメラ
	ADXCamera camera_{};

	//オブジェクト
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
	//全てのオブジェクト
	std::list<ADXObject*> objs{};
};