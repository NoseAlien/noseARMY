#pragma once

#include <windows.h>
#include <string>

//ウインドウを扱うクラス
class ADXWindow
{
public:
	RECT wrc_{};
	uint32_t window_width_ = 1280;
	uint32_t window_height_ = 720;

private:
	HWND hwnd_{};
	WNDCLASSEX w_{};

public:
	//コンストラクタ
	void Initialize(
		const LPCTSTR& window_title);

	//hwndを取得
	HWND GetHwnd() const { return hwnd_; }

	//hInstanceを取得
	HINSTANCE GetHInstance() const { return w_.hInstance; }

	//ウインドウのアスペクトを取得
	float GetAspect() { return (float)window_width_ / window_height_; };

	//xボタンが押されたか
	bool ProcessMessage();

	//終了処理
	void Finalize();

private:
	//コンストラクタ、デストラクタをprivateにしてシングルトンに

	ADXWindow() {};
	~ADXWindow() {};

	ADXWindow(const ADXWindow&) = delete;
	ADXWindow& operator=(const ADXWindow&) = delete;

private:
	static ADXWindow S_instance;

public:
	//ウインドウプロシージャ
	static LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

	//インスタンスを取得
	static ADXWindow* GetInstance() { return &S_instance; }
};