#pragma once

#include <windows.h>

class ADXWindow
{
public:
	static	LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);


public:
	static const int window_width = 1280;
	static const int window_height = 720;

	RECT wrc{};

	ADXWindow(
		LPCTSTR window_title);

	HWND GetHwnd() const { return hwnd; }
	HINSTANCE GetHInstance() const { return w.hInstance; }
	bool ProcessMessage();

	void Finalize();

private:
	HWND hwnd;
	WNDCLASSEX w{};
};