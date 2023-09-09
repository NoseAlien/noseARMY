#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include "ADXCommon.h"
#include "ADXWindow.h"
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
		ADXKeyBoardInput keyboard(&adxwindow);

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
			if (adxwindow.ProcessMessage() || keyboard.KeyTrigger(DIK_ESCAPE))
			{
				break;
			}

			adximgui.Begin();
			keyboard.Update();
			sceneMan.Update();
			ADXObject::StaticUpdate();
			//この下にあるゲーム専用の更新部分は後でまとめる
			SceneTransition::StaticUpdate();
			FieldBox::StaticUpdate();
			TutorialArea::StaticUpdate();
			LiveEntity::StaticUpdate();
			PlayerMini::StaticUpdate();
			ADXCollider::StaticUpdate();
			adximgui.End();

			adxcommon.PreDraw();
			ADXObject::PreDraw();
			ADXObject::StaticDraw();
			ADXObject::PostDraw();
			adximgui.Draw();
			adxcommon.PostDraw();
		}
		ADXAudio::StaticFinalize();
		adxwindow.Finalize();
		adximgui.Finalize();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}