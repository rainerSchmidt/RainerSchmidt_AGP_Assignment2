#include "renderer.h"

Renderer::Renderer(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11DepthStencilView* ZBuffer)
{
	g_pD3DDevice = D3DDevice;
	g_pImmediateContext = DeviceContext;
	g_pSwapChain = SwapChain;
	g_pBackBufferRTView = BackBuffer;
	g_pZBuffer = ZBuffer;
	
}

Renderer::~Renderer()
{
	//call CleanUp function
	CleanUp();
}

HRESULT Renderer::InitialiseGraphicsElements()
{
	//initialise Models

	//set up Scene Nodes

	return S_OK;
}

void Renderer::ClearBackBuffer()
{
	//// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
	g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::Draw()
{
	// Draw the vertex buffer to the back buffer //03-01
	OutputDebugString("Draw");

	//Draw Models
	//...

	//g_pImmediateContext->Draw(36, 0);
}

void Renderer::RenderFrame()
{
	ClearBackBuffer();
	/*SetLighting();
	SetVertexBuffer();
	SetWorldMatrix();
	SetLightWorldMatrix();
	SetConstantBuffer();
	SetWorldViewProjection();
	SetContext();*/
	Draw();



	//render all the stuff
	g_pSwapChain->Present(0, 0);
}

void Renderer::CleanUp()
{
	/*if (g_pVertexBuffer) g_pVertexBuffer->Release();
	if (g_pZBuffer) g_pZBuffer->Release();
	if (g_pTexture) g_pTexture->Release();
	if (g_pSampler) g_pSampler->Release();
	if (g_pSwapChain) g_pSwapChain->Release();
	if (g_pImmediateContext) g_pImmediateContext->Release();
	if (g_pD3DDevice) g_pD3DDevice->Release();
	if (g_pBackBufferRTView) g_pBackBufferRTView->Release();
	if (g_pTransformationBuffer) g_pTransformationBuffer->Release();
	if (g_pLightBuffer) g_pLightBuffer->Release();*/
	//if(g_pCamera) delete g_pCamera;
	
}