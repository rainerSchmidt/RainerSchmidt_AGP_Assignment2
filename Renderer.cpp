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

void Renderer::ClearBackBuffer()
{
	//// Clear the back buffer - choose a colour you like
	float rgba_clear_colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	g_pImmediateContext->ClearRenderTargetView(g_pBackBufferRTView, rgba_clear_colour);
	g_pImmediateContext->ClearDepthStencilView(g_pZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Renderer::SetLighting()
{
	//// set lighting
	//g_directional_light_shines_from = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	//g_directional_light_colour = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
	//g_ambient_light_colour = XMVectorSet(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::SetVertexBuffer()
{
	//// Set vertex buffer //03-01
	//UINT stride = sizeof(POS_COL_TEX_NORM_VERTEX);
	//UINT offset = 0;
	//g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	//g_pImmediateContext->PSSetSamplers(0, 1, &g_pSampler);
	//g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTexture);
}

void Renderer::SetWorldMatrix()
{
	// create and set WorldViewMatrix
	

	/*objectRotY += 0.01;
	if (objectRotY >= 360)
		objectRotY = 0;*/

	world = XMMatrixRotationZ(XMConvertToRadians(45));
	world *= XMMatrixRotationX(XMConvertToRadians(45));
	world *= XMMatrixRotationY(XMConvertToRadians(45));
	world *= XMMatrixTranslation(0.0f, 0.0f, 10.0f);
}

void Renderer::SetLightWorldMatrix()
{
	//XMMATRIX lightWorld;

	//// light rotation matrix
	//lightWorld = XMMatrixRotationX(XMConvertToRadians(lightRotY));
	//lightWorld *= XMMatrixRotationX(XMConvertToRadians(lightRotX));
	////lightWorld = XMMatrixRotationX(XMConvertToRadians(lightRotZ));
	//lightWorld *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}

void Renderer::SetConstantBuffer()
{
	// Set constant buffer

	/*transpose = XMMatrixTranspose(lightWorld);
	cb0_values.ThreeColorAmount = 0.5f;
	cb0_values.directional_light_colour = g_directional_light_colour;
	cb0_values.ambient_light_colour = g_ambient_light_colour;
	cb0_values.directional_light_vector = XMVector3Transform(g_directional_light_shines_from, transpose);
	cb0_values.directional_light_vector = XMVector3Normalize(cb0_values.directional_light_vector);*/
}

void Renderer::SetWorldViewProjection()
{
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 640.0f / 480.0f, 1.0f, 100.0f);
	// set view with camera values
	view = XMMatrixIdentity();
	//view = g_pCamera->GetViewMatrix();
	// set WorldViewProjection
	g_transBufferValues.WorldViewProjection = world * view * projection;
}

void Renderer::SetContext()
{
	//// upload the new values for the constant buffer
	//g_pImmediateContext->UpdateSubresource(g_pTransformationBuffer, 0, 0, &g_transBufferValues, 0, 0);
	//// Set constant buffer to active
	//g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pTransformationBuffer);

	//// Select which primitive type to use //03-01
	//g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Renderer::Draw()
{
	// Draw the vertex buffer to the back buffer //03-01
	OutputDebugString("Draw");
	g_pImmediateContext->Draw(36, 0);
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