#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include "ADXCommon.h"
#include "ADXWindow.h"
#include "ADXKeyBoardInput.h"
#include "ADXGamePadInput.h"
#include "ADXKeyConfig.h"
#include "ADXScene.h"
#include "ADXAudio.h"
#include "ADXImGuiManager.h"

#include "SceneManager.h"
#include "SceneTransition.h"
#include "FieldBox.h"
#include "TutorialArea.h"
#include "LiveEntity.h"
#include "PlayerMini.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		//WinAPI初期化処理

		//ウィンドウ生成
		ADXWindow adxwindow(L"noseARMY");

		ADXCommon adxcommon;
		adxcommon.Initialize(&adxwindow);

		//キーボードデバイスの生成
		ADXKeyBoardInput keyboard_(&adxwindow);
		ADXGamePadInput gamePad_;
		ADXKeyConfig config_;
		config_.AddConfigCell({ "jump", {DIK_SPACE}, {B}});
		config_.AddConfigCell({ "attack", {DIK_Z,DIK_X,DIK_C,DIK_V,DIK_B}, {A}});
		config_.AddVecConfigCell({ "vec",
			{DIK_UP,DIK_W},{DIK_DOWN,DIK_S},{DIK_RIGHT,DIK_D},{DIK_LEFT,DIK_A},
			{},{},{},{},
			{LEFT} });
		config_.AddConfigCell({ "select", {DIK_SPACE}, {B}});
		config_.AddConfigCell({ "back", {DIK_Z,DIK_X,DIK_C,DIK_V,DIK_B}, {A}});
		config_.AddConfigCell({ "return", {DIK_Q,DIK_BACKSPACE,DIK_RETURN}, {}});

		ADXImGuiManager adximgui;
		adximgui.Initialize(&adxwindow);

		ADXAudio::StaticInitialize();
		ADXImage::StaticInitialize();
		ADXObject::StaticInitialize();
		SceneTransition::StaticInitialize();

		//ゲームシーンの作成
		SceneManager sceneMan{};
		sceneMan.Initialize();

		//ゲームループ
		while (true)
		{
			if (adxwindow.ProcessMessage() || keyboard_.GetKeyDown(DIK_ESCAPE))
			{
				break;
			}

			adximgui.Begin();
			keyboard_.Update();
			gamePad_.Update();
			config_.Update();
			sceneMan.Update();
			ADXObject::StaticUpdate();
			//この下にあるゲーム専用の更新部分は後でまとめる
			SceneTransition::StaticUpdate();
			LiveEntity::StaticUpdate();
			ADXCollider::StaticUpdate();
			adximgui.End();

			adxcommon.PreDraw();
			ADXObject::StaticDraw();
			adximgui.StaticDraw();
			adxcommon.PostDraw();
		}
		ADXAudio::StaticFinalize();
		adxwindow.Finalize();
		adximgui.Finalize();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}