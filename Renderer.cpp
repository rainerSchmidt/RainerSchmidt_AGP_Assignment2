#include "renderer.h"

Renderer::Renderer(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
					ID3D11DepthStencilView* ZBuffer, Input* Input)
{
	g_pD3DDevice = D3DDevice;
	g_pImmediateContext = DeviceContext;
	g_pSwapChain = SwapChain;
	g_pBackBufferRTView = BackBuffer;
	g_pZBuffer = ZBuffer;
	g_pInput = Input;
	g_pCamera = new Camera(0.0, 0.0, 0.0, 0.0);
	
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

	//load textures from file
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "assets/sword.png", NULL, NULL, &g_pTexture0, NULL)))
	{
		OutputDebugString("There was an error loading the texture file!");
	}
	else
	{
		g_pModel->SetTexture(g_pTexture0);
	}
	
	//set up Scene Nodes
	g_pRootNode = new SceneNode(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	g_pEnemiesNode = new SceneNode(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	g_pPlayer = new SceneNode(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	g_pPlayer->SetModel(g_pModel);

	g_pEnemy = new SceneNode(3.0, 0.0, 10.0, 0.0, 0.0, 0.0, 1.0);
	g_pEnemy->SetModel(g_pModel);
	g_pEnemy2 = new SceneNode(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 1.0);
	g_pEnemy2->SetModel(g_pModel);
	
	g_pRootNode->AddChildNode(g_pEnemiesNode);
	g_pRootNode->AddChildNode(g_pPlayer);

	g_pEnemiesNode->AddChildNode(g_pEnemy);
	g_pEnemiesNode->AddChildNode(g_pEnemy2);

	//set Text with font from file
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

	// Draw the vertex buffer to the back buffer //03-01
	OutputDebugString("Draw");

	/*XMMATRIX world, view, projection;*/
	XMMATRIX view, projection;

	/*world = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	world *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	world *= XMMatrixRotationY(XMConvertToRadians(0.0f));
	world *= XMMatrixRotationZ(XMConvertToRadians(0.0f));
	world *= XMMatrixTranslation(0.0f, 0.0f, 15.0f);*/

	view = XMMatrixIdentity();
	view = g_pCamera->GetViewMatrix();

	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 800.0f / 600.0f, 0.01f, 100.0f);

	////Draw Models
	//g_pModel->Draw(&world, &view, &projection);

	//Execute RootNode
	g_pRootNode->Execute(&XMMatrixIdentity(), &view, &projection);

	//g_pImmediateContext->Draw(36, 0);
}

void Renderer::FrameLimit()
{
	/////////////////////////////////////////////////////////
	//// The Following Code is from the user HolyBlackCat from the forum stackoverflow.com.
	//// The post was published on the 2nd August, 2016
	////////////////////////////////////////////////////////

	//set startTime
	startTime = chrono::system_clock::now();

	chrono::duration<double, milli> frameDuration = startTime - endTime;
	chrono::milliseconds sleepTime;
	if (frameDuration.count() < 16.666)
	{
		std::chrono::duration<double, std::milli> delta_ms(16.666 - frameDuration.count());
		sleepTime = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime.count()));
	}

	endTime = chrono::system_clock::now();

	//show frames
	g_pText2D->AddText("Frames: " + to_string(1000 / (frameDuration + sleepTime).count()), -1.0f, 1.0f, 0.05f);
	g_pText2D->RenderText();
}

void Renderer::RenderFrame()
{

	
	//Gravity();
	ClearBackBuffer();
	/*SetLighting();
	SetVertexBuffer();
	SetWorldMatrix();
	SetLightWorldMatrix();
	SetConstantBuffer();
	SetWorldViewProjection();*/

	// Select which primitive type to use //03-01
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_pInput->KeyLogic(g_pCamera, g_pRootNode, g_pPlayer, g_pEnemiesNode);
	g_pInput->MouseLogic(g_pCamera, g_pPlayer, g_pRootNode);
	FrameLimit();
	Draw();

	

	//render all the stuff
	g_pSwapChain->Present(0, 0);
}

void Renderer::Gravity()
{
	//apply gravity to vertical velocity
		g_pCamera->AddVelocityY(g_gravity);

	//only call MoveUp function when velocity is not zero
	if (g_pCamera->GetVelocityY() != 0.0f)
	{
		if (!g_pPlayer->CheckCollisionRay(g_pPlayer,
			0.0f, g_pCamera->GetVelocityY(), 0.0f))
		{
			g_pPlayer->MoveUp(g_pCamera->GetVelocityY(), g_pRootNode, false);
			g_pCamera->MoveUp();
		}
			 
		/*if (!g_pPlayer->MoveUp(g_pCamera->GetVelocityY(), g_pRootNode))
			g_pCamera->MoveUp();*/
	}
		

	//if player is on the ground set vertical velocity to zero
	if (g_pPlayer->GetY() <= 0.0f)
	{
		g_pInput->SetPressed(false);
 		g_pCamera->SetVelocityY(0.0f);

		g_pPlayer->SetPosY(0.0f);
		g_pCamera->SetY(0.0f);

	}
		

	
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
	if(g_pCamera) delete g_pCamera;


	if (g_pTexture0) g_pTexture0->Release();
	if (g_pModel) delete g_pModel;

	////detach child nodes from RootNode and delete child nodes
	//if (g_pPlayer)
	//{
	//	g_pRootNode->DetatchNode(g_pPlayer);
	//	delete g_pPlayer;
	//}
	//if (g_pEnemy)
	//{
	//	delete g_pEnemy;
	//}

	////delete RootNode
	//if (g_pRootNode) delete g_pRootNode;

	//if (g_pText2D) delete g_pText2D;
	
}