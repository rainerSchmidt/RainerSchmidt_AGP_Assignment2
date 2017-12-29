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
	g_pModel = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModel->LoadObjModel("assets/cube.obj");

	if (FAILED(D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "assets/sword.png", NULL, NULL, &g_pTexture0, NULL)))
	{
		OutputDebugString("There was an error loading the texture file!");
	}
	else
	{
		g_pModel->SetTexture(g_pTexture0);
	}
	
	//set up Scene Nodes

	g_pText2D = new Text2D("assets/font1.bmp", g_pD3DDevice, g_pImmediateContext);
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
	g_pText2D->AddText("Time", -1.0f, 1.0f, 0.2f);
	// Draw the vertex buffer to the back buffer //03-01
	OutputDebugString("Draw");

	XMMATRIX world, view, projection;

	world = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	world *= XMMatrixRotationY(XMConvertToRadians(0.0f));
	world *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	world *= XMMatrixTranslation(0.0f, 0.0f, 15.0f);

	view = XMMatrixIdentity();

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 800.0f / 600.0f, 1.0f, 100.0f);

	//Draw Text
	g_pText2D->RenderText();

	//Draw Models
	g_pModel->Draw(&world, &view, &projection);

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
	SetWorldViewProjection();*/

	// Select which primitive type to use //03-01
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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


	if (g_pTexture0) g_pTexture0->Release();
	if (g_pModel) delete g_pModel;
	if (g_pText2D) delete g_pText2D;
	
}