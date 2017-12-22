#include "WindowInput.h"

LRESULT CALLBACK WindowInput::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
	
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	
			// 0x44 = 'a'
	
			//case WM_KEYDOWN:
			//	if (wParam == VK_ESCAPE)
			//		DestroyWindow(g_hWnd);
			//	if (wParam == 0x41) // Key 'a'
			//		g_pCamera->Strafe(-0.5);
			//	if (wParam == 0x44) // Key 'd'
			//		g_pCamera->Strafe(0.5);
			//	if (wParam == 0x57) // Key 'w'
			//		g_pCamera->Forward(1);
			//	if (wParam == 0x53) // Key 's'
			//		g_pCamera->Forward(-1);
			//	if (wParam == 0x45) // Key 'e'
			//		g_pCamera->Rotate(2);
			//	if (wParam == 0x51) // Key 'q'
			//		g_pCamera->Rotate(-2);
			//	if (wParam == VK_RIGHT)
			//		lightRotY += 2;
			//	if (wParam == VK_LEFT)
			//		lightRotY -= 2;
			//	if (wParam == VK_UP)
			//		lightRotX += 2;
			//	if (wParam == VK_DOWN)
			//		lightRotX += -2;
			//	return 0;
	
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

WindowInput::WindowInput()
{

}