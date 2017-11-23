//#include <windows.h>
//#include <d3d11.h>
//#include <d3dx11.h>
//#include <dxerr.h>
//#include <stdio.h>
//#define _XM_NO_INTINSICS_
//#define XM_NO_ALIGNMENT
//#include <xnamath.h>

#include "init.h";
#include "datamanager.h";


//////////////////////////////////////////////////////////////////////////////////////
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//////////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	//create object instances
	Init* init = new Init();
	DataManager* datamanager = new DataManager();

	//initialise application window
	if (FAILED(init->InitialiseWindow(hInstance, nCmdShow)))
	{
		DXTRACE_MSG("Failed to create Window");
		return 0;
	}

	//initialise D3D objects
	if (FAILED(init->InitialiseD3D()))
	{
		DXTRACE_MSG("Failed to create Device");
		return 0;
	}

	//initialise graphics elements
	if (FAILED(init->InitialiseGraphics()))
	{
		DXTRACE_MSG("Failed to initialise graphics");
		return 0;
	}


	// Main message loop
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// do something
			datamanager->Render();
		}
	}

	//clean up d3d objects
	delete datamanager;
	delete init;

	return (int)msg.wParam;
}