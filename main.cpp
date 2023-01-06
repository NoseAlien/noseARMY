#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include "ADXCommon.h"
#include "ADXWindow.h"
#include "ADXScene.h"
#include "ADXAudio.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		//WinAPI初期化処理

		//ウィンドウ生成
		ADXWindow adxwindow(L"ADXEngine");

		HRESULT result;

		ADXCommon adxcommon;
		adxcommon.Initialize(&adxwindow);

		//キーボードデバイスの生成
		ADXKeyBoardInput keyboard(&adxwindow);

		ADXAudio::StaticInitialize();
		ADXImage::StaticInitialize(adxcommon.GetDevice());
		ADXModel::StaticInitialize(adxcommon.GetDevice());
		ADXObject::StaticInitialize(adxcommon.GetDevice(), adxcommon.GetDsvHandle());
		ADXCamera::StaticInitialize(&ADXWindow::window_width, &ADXWindow::window_height);

		//ゲームシーンの作成
		ADXScene scene{};
		scene.Initialize(&keyboard, adxcommon.GetDevice());


		//ゲームループ
		while (true)
		{
			if (adxwindow.ProcessMessage())
			{
				break;
			}

			keyboard.Update();
			scene.Update();
			ADXObject::StaticUpdate();
			//この下にあるゲーム専用の更新部分は後でまとめる
			TutorialArea::StaticUpdate();
			ADXCollider::CollidersUpdate();

			adxcommon.PreDraw();
			ADXObject::PreDraw(adxcommon.GetCommandList());
			ADXObject::StaticDraw(ADXImage::GetSrvHeap());
			ADXObject::PostDraw();
			adxcommon.PostDraw();
		}
		ADXAudio::xAudio2.Reset();
		adxwindow.Finalize();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}