#pragma once

#include <windows.h>
#include <string>

class ADXWindow
{
public:
	static	LRESULT WindowProc(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);


public:
	static uint32_t S_window_width;
	static uint32_t S_window_height;

	RECT wrc{};

	ADXWindow(
		const LPCTSTR& window_title);

	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	bool ProcessMessage();

	void Finalize();

private:
	HWND hwnd;
	WNDCLASSEX w{};
};