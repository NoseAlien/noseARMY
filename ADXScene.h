#pragma once

#include "ADXKeyBoardInput.h"
#include "ADXModel.h"
#include "ADXMaterial.h"
#include "ADXWorldTransform.h"
#include "ADXObject.h"
#include "ADXCamera.h"
#include "Player.h"
#include "ADXImage.h"
#include "ADXTexAnimation.h"
#include <typeinfo>

class ADXScene
{
public:
	ADXKeyBoardInput* keyboard = nullptr;
	ID3D12Device* device = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE* srvHandle = nullptr;
	UINT64* CpuStartHandle = nullptr;
	UINT* incrementSize = nullptr;

	//画像
	ADXImage napnoseImg{};
	ADXImage MEGNOSE_Img{};
	ADXImage apEGnoSE_Image{};
	ADXImage skyDomeImg{};
	ADXImage whiteDotImg{};

	//3Dモデル
	ADXModel cube{};
	ADXModel rect{};
	ADXModel playerModel{};
	ADXModel skyDomeModel{};

	//マテリアル
	ADXMaterial material{};
	ADXMaterial unlitMat{};
	ADXMaterial lightShadeMat{};

	//カメラ
	ADXCamera camera_{};

	//オブジェクト
	Player player_{};
	ADXObject object_{};
	ADXObject object2_{};
	ADXObject object3_{};
	ADXObject skyDome_{};
	ADXObject sprite_{};

public:
	ADXScene();
	void Initialize(ADXKeyBoardInput* setKeyboard, ID3D12Device* setDevice);
	void Update();

private:
	//全てのオブジェクト
	std::vector<ADXObject*> objs{};
};