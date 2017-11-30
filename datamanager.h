#pragma once
#include "renderer.h"
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <dxerr.h>
#include <stdio.h>
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
//#include <xnamath.h>
//#include <DirectXMath.h>
//#include <DirectXPackedVector.h>
#include "player.h"

class DataManager
{
	public:
		DataManager(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11Buffer* VertexBuffer, ID3D11Buffer* TransformationBuffer, ID3D11Buffer* LightBuffer, ID3D11DepthStencilView* ZBuffer,
					ID3D11ShaderResourceView* Texture, ID3D11SamplerState* Sampler ,Player* Player//, XMVECTOR DirectionalLightDirection, XMVECTOR DirectionalLightColor, XMVECTOR AmbientLightColor);
					);
		~DataManager();
		void Render();
	private:
		Renderer* g_pRenderer;
		void CleanUp();
};