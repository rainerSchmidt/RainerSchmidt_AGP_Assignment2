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
#include "player.h"
#include "WindowInput.h"

using namespace DirectX;

class Init
{
	public:
		HRESULT InitialiseWindow(HINSTANCE hInstance, int nCmdShow, WNDPROC WdwProc);
		HRESULT InitialiseD3D();
		HRESULT InitialiseGraphics();

		Init();
		~Init();
		void ShutdownD3D();

		//Getter
		ID3D11Device*				GetDevice();
		ID3D11DeviceContext*		GetDeviceContext();
		ID3D11RenderTargetView*		GetBackBuffer();
		ID3D11Buffer*				GetVertexBuffer();
		ID3D11Buffer*				GetTransformationBuffer();
		ID3D11Buffer*				GetLightBuffer();
		ID3D11DepthStencilView*		GetZBuffer();
		ID3D11SamplerState*			GetSampler();
		ID3D11ShaderResourceView*	GetTexture();
		IDXGISwapChain*				GetSwapChain();
		Player*						GetPlayer();
		//XMVECTOR					GetDirectionalLightDirection();
		//XMVECTOR					GetDirectionalLightColor();
		//XMVECTOR					AmbientLightColor();



	private:
		
		HINSTANCE g_hInst;
		HWND	  g_hWnd;

		D3D_DRIVER_TYPE					g_driverType = D3D_DRIVER_TYPE_NULL;
		D3D_FEATURE_LEVEL				g_featureLevel = D3D_FEATURE_LEVEL_11_0;
		ID3D11Device*					g_pD3DDevice = NULL;
		ID3D11DeviceContext*			g_pImmediateContext = NULL;
		IDXGISwapChain*					g_pSwapChain = NULL;

		ID3D11RenderTargetView*			g_pBackBufferRTView = NULL;
		
		ID3D11Buffer*					g_pVertexBuffer;
		ID3D11VertexShader*				g_pVertexShader;
		ID3D11PixelShader*				g_pPixelShader;
		ID3D11InputLayout*				g_pInputLayout;
		ID3D11Buffer*					g_pTransformationBuffer;
		ID3D11Buffer*					g_pLightBuffer;
		ID3D11ShaderResourceView*		g_pTexture; // Array?
		ID3D11SamplerState*				g_pSampler; // Array?
		ID3D11DepthStencilView*			g_pZBuffer;

		//Lightvectors(directional_light_shines_from, directional_light_color, ambient_light_color)

		struct POS_COL_TEX_NORM_VERTEX
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Col;
			XMFLOAT2 Tex;
			XMFLOAT3 Normal;
		};
		


};


