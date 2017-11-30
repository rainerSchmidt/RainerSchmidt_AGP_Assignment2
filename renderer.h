#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
//#include <xnamath.h>
#include "player.h"

using namespace DirectX;

class Renderer
{
	public:
		Renderer(	ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11Buffer* VertexBuffer, ID3D11Buffer* TransformationBuffer, ID3D11Buffer* LightBuffer, ID3D11DepthStencilView* ZBuffer,
					ID3D11ShaderResourceView* Texture, ID3D11SamplerState* Sampler ,Player* Player//, XMVECTOR DirectionalLightDirection, XMVECTOR DirectionalLightColor, XMVECTOR AmbientLightColor);
				);
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
		//ID3D11VertexShader*				g_pVertexShader;
		//ID3D11PixelShader*				g_pPixelShader;
		//ID3D11InputLayout*				g_pInputLayout;
		ID3D11Buffer*					g_pTransformationBuffer;
		ID3D11Buffer*					g_pLightBuffer;
		ID3D11ShaderResourceView*		g_pTexture; // Array?
		ID3D11SamplerState*				g_pSampler; // Array?
		ID3D11DepthStencilView*			g_pZBuffer;

		Player*							g_pPlayer;
		//Lightvectors(directional_light_shines_from, directional_light_color, ambient_light_color)

		XMMATRIX projection, world, view;
		TransformationBuffer g_transBufferValues;
};


