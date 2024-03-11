#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include "ADXCommon.h"
#include "ADXWindow.h"
#include "ADXKeyBoardInput.h"
#include "ADXGamePadInput.h"
#include "ADXKeyConfig.h"
#include "ADXTextRenderer.h"
#include "ADXScene.h"
#include "ADXAudioSource.h"
#include "ADXImGuiManager.h"

#include "SceneManager.h"
#include "SceneTransition.h"
#include "LiveEntity.h"
#include "ShadowRenderer.h"

using namespace DirectX;
using namespace Microsoft::WRL;

int32_t WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int32_t)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		//WinAPI初期化処理

		//ウィンドウ生成
		ADXWindow::GetInstance()->Initialize(L"noseARMY");

		ADXCommon::GetInstance()->Initialize(ADXWindow::GetInstance());

		//キーボードデバイスの生成
		ADXKeyBoardInput keyboard_(ADXWindow::GetInstance());
		ADXGamePadInput gamePad_;

		ADXImGuiManager::GetInstance()->Initialize(ADXWindow::GetInstance());

		ADXAudioSource::StaticInitialize();
		ADXImage::StaticInitialize();
		ADXObject::StaticInitialize();
		SceneTransition::StaticInitialize();
		ShadowRenderer::UniqueInitialize();

		ADXKeyConfig config_;
		config_.AddConfigCell({ "jump", {DIK_SPACE}, {ADXGamePadInput::B,ADXGamePadInput::X} });
		config_.AddConfigCell({ "attack", {DIK_Z,DIK_X,DIK_C,DIK_V,DIK_B}, {ADXGamePadInput::A,ADXGamePadInput::Y} });
		config_.AddVecConfigCell({ "vec",
			{DIK_UP},{DIK_DOWN},{DIK_RIGHT},{DIK_LEFT},
			{},{},{},{},
			{ADXGamePadInput::LEFT} });
		config_.AddVecConfigCell({ "cameraTilt",
			{DIK_W},{DIK_S},{DIK_D},{DIK_A},
			{},{},{},{},
			{ADXGamePadInput::RIGHT} });
		config_.AddConfigCell({ "select", {DIK_SPACE}, {ADXGamePadInput::B} });
		config_.AddConfigCell({ "back", {DIK_Z,DIK_X,DIK_C,DIK_V,DIK_B}, {ADXGamePadInput::A} });
		config_.AddConfigCell({ "return", {DIK_Q,DIK_BACKSPACE,DIK_RETURN}, {ADXGamePadInput::BACK} });
		ADXTextRenderer::AddFont("0123456789qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM +-/,.:;_[]",
			"texture/alphaNumber", "large",
			{ {'+',"plus"},{'-',"minus"},{'/',"slash"} ,{' ',"space"},{',',"comma"},{'.',"period"},{':',"colon"},{';',"semicolon"},{'_',"underscore"} });

		//ゲームシーンの作成
		SceneManager sceneMan{};
		sceneMan.Initialize();

		//ゲームループ
		while (true)
		{
			if (ADXWindow::GetInstance()->ProcessMessage() || keyboard_.GetKeyDown(DIK_ESCAPE))
			{
				break;
			}

			ADXImGuiManager::GetInstance()->PreDraw();
			keyboard_.Update();
			gamePad_.Update();
			config_.Update();
			sceneMan.Update();
			ADXObject::StaticUpdate();
			//この下にあるゲーム専用の更新部分は後でまとめる
			SceneTransition::StaticUpdate();
			LiveEntity::StaticUpdate();
			ADXCollider::StaticUpdate();

			ADXCommon::GetInstance()->PreDraw();
			ADXObject::StaticDraw();
			ADXImGuiManager::GetInstance()->PostDraw();
			ADXImGuiManager::GetInstance()->StaticDraw();
			ADXCommon::GetInstance()->PostDraw();
		}
		ADXAudioSource::StaticFinalize();
		ADXWindow::GetInstance()->Finalize();
		ADXImGuiManager::GetInstance()->Finalize();
	}
	_CrtDumpMemoryLeaks();
	return 0;
}