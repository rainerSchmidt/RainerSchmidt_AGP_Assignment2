#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <stdio.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
//#include <xnamath.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;

class Init
{
	public:
		HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow);
		HRESULT InitialiseD3D();

		Init();
		~Init();
		void ShutdownD3D();

		//Getter
		ID3D11Device*				GetDevice();
		ID3D11DeviceContext*		GetDeviceContext();
		ID3D11RenderTargetView*		GetBackBuffer();
		ID3D11DepthStencilView*		GetZBuffer();
		IDXGISwapChain*				GetSwapChain();



	private:
		HINSTANCE g_hInst;
		HWND	  g_hWnd;

		D3D_DRIVER_TYPE					g_driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL				g_featureLevel = D3D_FEATURE_LEVEL_11_0;
		ID3D11Device*					g_pD3DDevice = NULL;
		ID3D11DeviceContext*			g_pImmediateContext = NULL;
		IDXGISwapChain*					g_pSwapChain = NULL;

		ID3D11RenderTargetView*			g_pBackBufferRTView = NULL;
		ID3D11DepthStencilView*			g_pZBuffer;
		


};


