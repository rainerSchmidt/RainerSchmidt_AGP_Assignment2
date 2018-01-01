#pragma once
#include <dinput.h>
#include "camera.h"
#include "scenenode.h"

class Input
{
private:

	IDirectInput8*			g_direct_input;
	IDirectInputDevice8*	g_keyboard_device;
	IDirectInputDevice8*	g_mouse_device;
	DIMOUSESTATE			g_mouse_state;
	unsigned char			g_keyboard_keys_state[256];

	HINSTANCE*				g_HInst;
	HWND*					g_HWnd;

	bool IsKeyPressed(unsigned char Keycode);
	void ReadInputStates();

	bool pressed = false;

public:
	Input(HINSTANCE* HInst, HWND* HWnd);
	~Input();
	HRESULT InitialiseInput();
	void KeyLogic(Camera* Cam, SceneNode* Node, SceneNode* RootNode);
	void MouseLogic(Camera* Cam, SceneNode* Node);
	void SetPressed(bool status);
};
