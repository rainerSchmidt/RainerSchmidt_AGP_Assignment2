#pragma once
#include <dinput.h>

class Input
{
private:

	IDirectInput8*			g_direct_input;
	IDirectInputDevice8*	g_keyboard_device;
	unsigned char			g_keyboard_keys_state[256];

	bool IsKeyPressed(unsigned char Keycode);
	void ReadInputStates();

public:
	Input();
	~Input();
	HRESULT InitialiseInput(HINSTANCE* HInst, HWND* HWnd);
	void KeyLogic();
};
