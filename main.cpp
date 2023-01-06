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
		//WinAPI����������

		//�E�B���h�E����
		ADXWindow adxwindow(L"ADXEngine");

		HRESULT result;

		ADXCommon adxcommon;
		adxcommon.Initialize(&adxwindow);

		//�L�[�{�[�h�f�o�C�X�̐���
		ADXKeyBoardInput keyboard(&adxwindow);

		ADXAudio::StaticInitialize();
		ADXImage::StaticInitialize(adxcommon.GetDevice());
		ADXModel::StaticInitialize(adxcommon.GetDevice());
		ADXObject::StaticInitialize(adxcommon.GetDevice(), adxcommon.GetDsvHandle());
		ADXCamera::StaticInitialize(&ADXWindow::window_width, &ADXWindow::window_height);

		//�Q�[���V�[���̍쐬
		ADXScene scene{};
		scene.Initialize(&keyboard, adxcommon.GetDevice());


		//�Q�[�����[�v
		while (true)
		{
			if (adxwindow.ProcessMessage())
			{
				break;
			}

			keyboard.Update();
			scene.Update();
			ADXObject::StaticUpdate();
			//���̉��ɂ���Q�[����p�̍X�V�����͌�ł܂Ƃ߂�
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