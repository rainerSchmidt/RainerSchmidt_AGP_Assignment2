#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
//#include <xnamath.h>

using namespace DirectX;

class Renderer
{
	public:
		Renderer(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11DepthStencilView* ZBuffer);
		void RenderFrame();
		~Renderer();
	private:

		void CleanUp();
		void ClearBackBuffer();
		void SetLighting();
		void SetVertexBuffer();
		void SetWorldMatrix();
		void SetLightWorldMatrix();
		void SetConstantBuffer();
		void SetWorldViewProjection();
		void SetContext();
		void Draw();

		struct TransformationBuffer
		{
			XMMATRIX WorldViewProjection;  // 64 Bytes
		};

		struct LightBuffer
		{
			XMVECTOR directional_light_vector; //16 Bytes
			XMVECTOR directional_light_color; //16 Bytes
			XMVECTOR ambient_light_color; //16 Bytes
		};

		struct POS_COL_TEX_NORM_VERTEX
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Col;
			XMFLOAT2 Tex;
			XMFLOAT3 Normal;
		};

		ID3D11Device*					g_pD3DDevice = NULL;
		ID3D11DeviceContext*			g_pImmediateContext = NULL;
		IDXGISwapChain*					g_pSwapChain = NULL;

		ID3D11RenderTargetView*			g_pBackBufferRTView = NULL;

		ID3D11Buffer*					g_pVertexBuffer;
		ID3D11DepthStencilView*			g_pZBuffer;

		DirectX::XMMATRIX projection, world, view;
		TransformationBuffer g_transBufferValues;
};


