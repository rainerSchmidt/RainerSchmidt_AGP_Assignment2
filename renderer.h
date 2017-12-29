#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
#include "model.h"
#include "text2D.h"
using namespace DirectX;

class Renderer
{
	public:
		Renderer(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11DepthStencilView* ZBuffer);

		HRESULT InitialiseGraphicsElements();
		void RenderFrame();
		~Renderer();
	private:
		
		void CleanUp();
		void ClearBackBuffer();
		void Draw();

		ID3D11Device*					g_pD3DDevice = NULL;
		ID3D11DeviceContext*			g_pImmediateContext = NULL;
		IDXGISwapChain*					g_pSwapChain = NULL;

		ID3D11RenderTargetView*			g_pBackBufferRTView = NULL;
		ID3D11DepthStencilView*			g_pZBuffer;

		//Models 
		Model*							g_pModel;
		
		//SceneNodes
		//...

		Text2D*							g_pText2D;
		//ID3D11VertexShader *	g_pVShader;
		//ID3D11PixelShader *		g_pPShader;
		//ID3D11InputLayout *		g_pInputLayout;
		//ID3D11Buffer *			g_pTransformationBuffer;
		//ID3D11Buffer *			g_pLightBuffer;
		ID3D11ShaderResourceView* g_pTexture0;
		//ID3D11SamplerState*		g_pSampler0;

		//DirectX::XMMATRIX projection, world, view;
		//TransformationBuffer g_transBufferValues;
};


