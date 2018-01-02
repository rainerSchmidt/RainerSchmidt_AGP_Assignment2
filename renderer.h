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
#include "scenenode.h"
#include "input.h"
#include "camera.h"
#include <chrono>
#include <thread>
using namespace DirectX;

class Renderer
{
	public:
		Renderer(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11DepthStencilView* ZBuffer, Input* Input);

		HRESULT InitialiseGraphicsElements();
		void RenderFrame();
		~Renderer();
	private:
		
		void FrameLimit();
		void CleanUp();
		void ClearBackBuffer();
		void Draw();
		void KeyLogic();
		void Gravity();

		ID3D11Device*					g_pD3DDevice = NULL;
		ID3D11DeviceContext*			g_pImmediateContext = NULL;
		//HWND*							g_pHWnd = NULL;
		IDXGISwapChain*					g_pSwapChain = NULL;

		ID3D11RenderTargetView*			g_pBackBufferRTView = NULL;
		ID3D11DepthStencilView*			g_pZBuffer;
		Input*							g_pInput;
		Camera*							g_pCamera;

		//Models 
		Model*							g_pModelPlane;
		Model*							g_pModelCube;
		Model*							g_pModelSphere;
		
		//SceneNodes
		SceneNode*						g_pRootNode;
		SceneNode*						g_pEnemiesNode;
		SceneNode*						g_pEnemy;
		SceneNode*						g_pEnemy2;
		SceneNode*						g_pPlayer;
		SceneNode*						g_pGround;

		chrono::system_clock::time_point startTime = chrono::system_clock::now();
		chrono::system_clock::time_point endTime = chrono::system_clock::now();
		Text2D*							g_pText2D;
		//ID3D11VertexShader *	g_pVShader;
		//ID3D11PixelShader *		g_pPShader;
		//ID3D11InputLayout *		g_pInputLayout;
		//ID3D11Buffer *			g_pTransformationBuffer;
		//ID3D11Buffer *			g_pLightBuffer;
		ID3D11ShaderResourceView* g_pTextureGrass;
		ID3D11ShaderResourceView* g_pTextureWall;
		ID3D11ShaderResourceView* g_pTextureTile;
		//ID3D11SamplerState*		g_pSampler0;

		//DirectX::XMMATRIX projection, world, view;
		//TransformationBuffer g_transBufferValues;

		//Gravity variables
		float g_gravity = -0.05f;
};


