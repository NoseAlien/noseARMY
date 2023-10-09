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
		//WinAPI����������

		//�E�B���h�E����
		ADXWindow adxwindow(L"noseARMY");

		ADXCommon adxcommon;
		adxcommon.Initialize(&adxwindow);

		//�L�[�{�[�h�f�o�C�X�̐���
		ADXKeyBoardInput keyboard(&adxwindow);

		ADXImGuiManager adximgui;
		adximgui.Initialize(&adxwindow);

		ADXAudio::StaticInitialize();
		ADXImage::StaticInitialize();
		ADXObject::StaticInitialize();
		SceneTransition::StaticInitialize();

		//�Q�[���V�[���̍쐬
		SceneManager sceneMan{};
		sceneMan.Initialize();

		//�Q�[�����[�v
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
			//���̉��ɂ���Q�[����p�̍X�V�����͌�ł܂Ƃ߂�
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