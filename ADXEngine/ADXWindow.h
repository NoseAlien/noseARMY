#pragma once

#include <windows.h>
#include <string>

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
	ADXWindow(
		const LPCTSTR& window_title);

	HWND GetHwnd() const { return hwnd_; }
	HINSTANCE GetHInstance() const { return w_.hInstance; }
	bool ProcessMessage();

	void Finalize();

public:
	static LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);
	static float GetAspect() { return (float)S_window_width / S_window_height; };
};