#pragma once

#include <windows.h>
#include <string>

class ADXWindow
{
public:
	RECT wrc{};

public:
	ADXWindow(
		const LPCTSTR& window_title);

	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	bool ProcessMessage();

	void Finalize();

private:
	HWND hwnd;
	WNDCLASSEX w{};

public:
	static uint32_t S_window_width;
	static uint32_t S_window_height;

public:
	static LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);
	static float GetAspect() { return (float)S_window_width / S_window_height; };
};