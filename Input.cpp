#include "input.h"

//Constructor: sets member variables with Parameters
Input::Input(HINSTANCE* HInst, HWND* HWnd)
{
	g_HInst = HInst;
	g_HWnd = HWnd;
}

//Desctructor: releases input devices
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

//Initialises Input decices for use in the application
//Taken from the Tutorials provided in AGP by the course leader Philip Alassad.
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

//Reads current input states for the devices
//Taken from the Tutorials provided in AGP by the course leader Philip Alassad.
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

//Returnes true if key that the KeyCode is assigned to is pressed.
//Taken from the Tutorials provided in AGP by the course leader Philip Alassad.
bool Input::IsKeyPressed(unsigned char Keycode)
{
	return g_keyboard_keys_state[Keycode] & 0x80;
}

//Logic for key inputs
//Uses player and camera nodes to move them
void Input::KeyLogic(Camera* Cam,SceneNode* RootNode, SceneNode* Node, SceneNode* Enemies)
{
	ReadInputStates();

	if (IsKeyPressed(DIK_ESCAPE))
		DestroyWindow(*g_HWnd);

	if (IsKeyPressed(DIK_W))
	{
		if (!Node->MoveForwards(g_MoveSpeed, RootNode, Enemies, Node, true))
			Cam->Forward(g_MoveSpeed);
	}
		
	if (IsKeyPressed(DIK_S))
	{
		if (!Node->MoveForwards(-g_MoveSpeed, RootNode, Enemies, Node, true))
			Cam->Forward(-g_MoveSpeed);
	}
	if (IsKeyPressed(DIK_A))
	{
		if (!Node->Strafe(-g_MoveSpeed, RootNode, Enemies, Node, true))
			Cam->Strafe(-g_MoveSpeed);
	}
	if (IsKeyPressed(DIK_D))
	{
		if (!Node->Strafe(g_MoveSpeed, RootNode, Enemies, Node, true))
			Cam->Strafe(g_MoveSpeed);
	}
	if (IsKeyPressed(DIK_SPACE))
	{ 
		if (!pressed)
		{
			pressed = true;
			Cam->AddVelocityY(0.75f);
			if (!Node->MoveUp(Cam->GetVelocityY(), RootNode, Enemies, Node, true))
				Cam->MoveUp();

			XMMATRIX identity = XMMatrixIdentity();
			RootNode->UpdateCollisionTree(&identity, 1.0f, Node);
		}
	}
}

//Logic for mouse input
//only rotates the y-axis
void Input::MouseLogic(Camera* Cam, SceneNode* Node, SceneNode* RootNode)
{
	Cam->Rotate(g_mouse_state.lX* 0.5f);
	Node->Rotate(g_mouse_state.lX*0.5f, RootNode, Node);
}

void Input::SetPressed(bool Status) { pressed = Status; }

