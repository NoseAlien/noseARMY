#include "ADXWindow.h"
#include <imgui_impl_win32.h>

uint32_t ADXWindow::S_window_width = 1280;
uint32_t ADXWindow::S_window_height = 720;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ADXWindow::ADXWindow(const LPCTSTR& window_title)
{
	w_.cbSize = sizeof(WNDCLASSEX);
	w_.lpfnWndProc = (WNDPROC)WindowProc;
	w_.lpszClassName = window_title;
	w_.hInstance = GetModuleHandle(nullptr);
	w_.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&w_);

	wrc_ = { 0,0,(LONG)S_window_width,(LONG)S_window_height };

	AdjustWindowRect(&wrc_, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(w_.lpszClassName,
		window_title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc_.right - wrc_.left,
		wrc_.bottom - wrc_.top,
		nullptr,
		nullptr,
		w_.hInstance,
		nullptr);

	ShowWindow(hwnd_, SW_SHOW);
}

LRESULT ADXWindow::WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{
	//ImGui用ウインドウプロシージャ呼び出し
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
	{
		return true;
	}

	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool ADXWindow::ProcessMessage()
{
	//メッセージ
	MSG msg{};

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//xボタンが押されたらループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}

	return false;
}


void ADXWindow::Finalize()
{
	UnregisterClass(w_.lpszClassName, w_.hInstance);
}