#pragma once
#include <windows.h>

class WindowInput
{
public:
	WindowInput();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
