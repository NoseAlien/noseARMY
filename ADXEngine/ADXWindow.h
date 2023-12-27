#pragma once

#include <windows.h>
#include <string>

//ウインドウを扱うクラス
class ADXWindow
{
public:
	RECT wrc_{};

private:
	HWND hwnd_{};
	WNDCLASSEX w_{};

public:
	static uint32_t S_window_width;
	static uint32_t S_window_height;

public:
	//コンストラクタ
	ADXWindow(
		const LPCTSTR& window_title);

	//hwndを取得
	HWND GetHwnd() const { return hwnd_; }

	//hInstanceを取得
	HINSTANCE GetHInstance() const { return w_.hInstance; }

	//xボタンが押されたか
	bool ProcessMessage();

	//終了処理
	void Finalize();

public:
	//ウインドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

	//ウインドウのアスペクトを表示
	static float GetAspect() { return (float)S_window_width / S_window_height; };
};