#include "input.h"

Input::Input(HINSTANCE* HInst, HWND* HWnd)
{
	g_HInst = HInst;
	g_HWnd = HWnd;
}

Input::~Input()
{
	if (g_keyboard_device)
	{
		g_keyboard_device->Unacquire();
		g_keyboard_device->Release();
	}

	if (g_mouse_device)
	{
		g_mouse_device->Unacquire();
		g_mouse_device->Release();
	}
}

HRESULT Input::InitialiseInput()
{
	HRESULT hr;

	ZeroMemory(g_keyboard_keys_state, sizeof(g_keyboard_keys_state));
	hr = DirectInput8Create(*g_HInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_direct_input, NULL);
	if (FAILED(hr))
		return hr;

	// create, initialise and set keyboard device
	hr = g_direct_input->CreateDevice(GUID_SysKeyboard, &g_keyboard_device, NULL);
	if (FAILED(hr))
		return hr;

	hr = g_keyboard_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
		return hr;

	hr = g_keyboard_device->SetCooperativeLevel(*g_HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = g_keyboard_device->Acquire();
	if (FAILED(hr))
		return hr;

	// create, initialise and set mouse device
	hr = g_direct_input->CreateDevice(GUID_SysMouse, &g_mouse_device, NULL);
	if (FAILED(hr))
		return hr;

	hr = g_mouse_device->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return hr;

	hr = g_mouse_device->SetCooperativeLevel(*g_HWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hr))
		return hr;

	hr = g_mouse_device->Acquire();
	if (FAILED(hr))
		return hr;



	return S_OK;
}

void Input::ReadInputStates()
{
	HRESULT hr;

	hr = g_keyboard_device->GetDeviceState(sizeof(g_keyboard_keys_state), (LPVOID)&g_keyboard_keys_state);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			g_keyboard_device->Acquire();
		}
	}

	hr = g_mouse_device->GetDeviceState(sizeof(g_mouse_state), (LPVOID)&g_mouse_state);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			g_mouse_device->Acquire();
		}
	}
}

bool Input::IsKeyPressed(unsigned char Keycode)
{
	return g_keyboard_keys_state[Keycode] & 0x80;
}

void Input::KeyLogic(Camera* Cam, SceneNode* Node, SceneNode* RootNode)
{
	ReadInputStates();

	if (IsKeyPressed(DIK_ESCAPE))
		DestroyWindow(*g_HWnd);

	if (IsKeyPressed(DIK_W))
	{
		if (!Node->MoveForwards(0.5f, RootNode))
			Cam->Forward(0.5f);
	}
		
	if (IsKeyPressed(DIK_S))
	{
		if (!Node->MoveForwards(-0.5f, RootNode))
			Cam->Forward(-0.5f);
	}
	if (IsKeyPressed(DIK_A))
	{
		if (!Node->Strafe(-0.5f, RootNode))
			Cam->Strafe(-0.5f);
	}
	if (IsKeyPressed(DIK_D))
	{
		if (!Node->Strafe(0.5f, RootNode))
			Cam->Strafe(0.5f);
	}
	if (IsKeyPressed(DIK_SPACE))
	{ //make the camera/player jump
	}
}

void Input::MouseLogic(Camera* Cam, SceneNode* Node)
{
	Cam->Rotate(g_mouse_state.lX);
	//Node->Rotae(g_mouse_state.lX);
}
