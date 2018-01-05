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
	g_pCamera = new Camera(0.0, 0.0, 3.0, 0.0);
	g_pCollectable = new Collectable();
	
}

Renderer::~Renderer()
{
	//call CleanUp function
	CleanUp();
}

HRESULT Renderer::InitialiseGraphicsElements()
{
	//initialise light values
	g_pLight = new Light();
	g_pLight->InitialiseLighting();

	//initialise Models
	g_pModelPlane = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModelPlane->LoadObjModel("assets/cube.obj");
	g_pModelPlane->SetLight(g_pLight->GetDirectionalLightColor(), g_pLight->GetDirectionalLightShinesFrom(), g_pLight->GetAmbientLightColor());

	g_pModelCube = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModelCube->LoadObjModel("assets/cube.obj");
	g_pModelCube->SetLight(g_pLight->GetDirectionalLightColor(), g_pLight->GetDirectionalLightShinesFrom(), g_pLight->GetAmbientLightColor());

	g_pModelSphere = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModelSphere->LoadObjModel("assets/Sphere.obj");
	g_pModelSphere->SetLight(g_pLight->GetDirectionalLightColor(), g_pLight->GetDirectionalLightShinesFrom(), g_pLight->GetAmbientLightColor());

	g_pModelCoin = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModelCoin->LoadObjModel("assets/edgedcoin.obj");
	g_pModelCoin->SetLight(g_pLight->GetDirectionalLightColor(), g_pLight->GetDirectionalLightShinesFrom(), g_pLight->GetAmbientLightColor());

	g_pModelGate = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModelGate->LoadObjModel("assets/doorway.obj");
	g_pModelGate->SetLight(g_pLight->GetDirectionalLightColor(), g_pLight->GetDirectionalLightShinesFrom(), g_pLight->GetAmbientLightColor());

	g_pModelPyramid = new Model(g_pD3DDevice, g_pImmediateContext);
	g_pModelPyramid->LoadObjModel("assets/reversepyramid.obj");
	g_pModelPyramid->SetLight(g_pLight->GetDirectionalLightColor(), g_pLight->GetDirectionalLightShinesFrom(), g_pLight->GetAmbientLightColor());

	//load grass texture from file
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "assets/grass.jpg", NULL, NULL, &g_pTextureGrass, NULL)))
		OutputDebugString("There was an error loading the texture file!");
	else
	{
		g_pModelPlane->SetTexture(g_pTextureGrass);
		g_pModelCoin->SetTexture(g_pTextureGrass);
	}
		

	//load wall texture from file
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "assets/wall.jpg", NULL, NULL, &g_pTextureWall, NULL)))
		OutputDebugString("There was an error loading the texture file!");
	else
	{
		g_pModelCube->SetTexture(g_pTextureWall);
		g_pModelPyramid->SetTexture(g_pTextureWall);
		g_pModelGate->SetTexture(g_pTextureWall);
	}
		

	//load tile texture from file
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(g_pD3DDevice, "assets/tiles.jpg", NULL, NULL, &g_pTextureTile, NULL)))
		OutputDebugString("There was an error loading the texture file!");
	else
	{
		g_pModelGate->SetTexture(g_pTextureTile);
		g_pModelSphere->SetTexture(g_pTextureTile);
	}
		
	
	//set up Scene Nodes
	//RootNode
	g_pRootNode = new SceneNode(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	//Collideables
	g_pCollideable = new SceneNode(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	//Enemies
	g_pEnemies[0] = new SceneNode(11.0f, -1.2f, 31.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pEnemies[1] = new SceneNode(11.0f, -1.2f, 46.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pEnemies[2] = new SceneNode(-7.0f, -1.2f, 45.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pEnemies[3] = new SceneNode(-9.0f, -1.2f, 79.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	//Collectables
	g_pCollectables[0] = new SceneNode(-5.0f, -0.2f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[1] = new SceneNode(-9.0f, -0.2f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[2] = new SceneNode(11.0f, -0.2f, 8.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[3] = new SceneNode(11.0f, -0.2f, 11.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[4] = new SceneNode(11.0f, -0.2f, 14.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[5] = new SceneNode(11.0f, -0.2f, 17.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[6] = new SceneNode(11.0f, -0.2f, 20.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[7] = new SceneNode(-4.0f, -0.2f, 32.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[8] = new SceneNode(-13.0f, -0.2f, 40.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[9] = new SceneNode(10.0f, -0.2f, 80.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[10] = new SceneNode(10.0f, -0.2f, 83.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pCollectables[11] = new SceneNode(10.0f, -0.2f, 86.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	//Obstacles
	g_pObstacles[0] = new SceneNode(3.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 5.0f);
	g_pObstacles[1] = new SceneNode(-2.0f, 0.0f, 38.0f, 0.0f, 0.0f, 0.0f, 3.0f, 1.0f, 1.0f);
	g_pObstacles[2] = new SceneNode(-8.0f, 0.0f, 34.0f, 0.0f, 0.0f, 0.0f, 3.0f, 1.0f, 1.0f);
	g_pObstacles[3] = new SceneNode(12.0f, 0.0f, 67.0f, 0.0f, 0.0f, 0.0f, 7.0f, 1.0f, 1.0f);
	g_pObstacles[4] = new SceneNode(-5.0f, 0.0f, 67.0f, 0.0f, 0.0f, 0.0f, 7.0f, 1.0f, 1.0f);

	//Moveables
	g_pMoveable[0] = new SceneNode(3.5f, 0.0f, 67.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

	//PlayerNode
	g_pPlayer = new SceneNode(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pPlayer->SetModel(g_pModelCube);

	//EndNode
	g_pEndNode = new SceneNode(0.0f, -1.2f, 98.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f);
	g_pEndNode->SetModel(g_pModelGate);

	//Decorations
	g_pDecorations = new SceneNode(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
	g_pGround = new SceneNode(0.0f, -1.2f, 0.0f, 0.0f, 0.0f, 0.0f, 20.0f, 0.2f, 100.0f);
	g_pGround->SetModel(g_pModelPlane);
	
	//Append Child Nodes for RootNode
	g_pRootNode->AddChildNode(g_pCollideable);
	g_pRootNode->AddChildNode(g_pDecorations);
	g_pRootNode->AddChildNode(g_pPlayer);

	for (int i = 0; i < ARRAYSIZE(g_pEnemies); i++) //Enemies
	{
		g_pCollideable->AddChildNode(g_pEnemies[i]);
		g_pEnemies[i]->SetModel(g_pModelPyramid);
		g_pEnemies[i]->SetTag(g_pEnemies[i]->Enemy);

		SceneNode* Node = new SceneNode(0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f);
		Node->SetModel(g_pModelSphere);

		g_pEnemies[i]->AddChildNode(Node);
	}

	for (int i = 0; i < ARRAYSIZE(g_pCollectables); i++) //Collectables
	{
		g_pCollideable->AddChildNode(g_pCollectables[i]);
		g_pCollectables[i]->SetModel(g_pModelCoin);
		g_pCollectables[i]->SetTag(g_pCollectables[i]->Collectable);
	}

	for (int i = 0; i < ARRAYSIZE(g_pObstacles); i++) //Obstacles
	{
		g_pCollideable->AddChildNode(g_pObstacles[i]);
		g_pObstacles[i]->SetModel(g_pModelCube);
		g_pObstacles[i]->SetTag(g_pObstacles[i]->Block);
	}

	for (int i = 0; i < ARRAYSIZE(g_pMoveable); i++) //Moveables
	{
		g_pCollideable->AddChildNode(g_pMoveable[i]);
		g_pMoveable[i]->SetModel(g_pModelCube);
		g_pMoveable[i]->SetTag(g_pMoveable[i]->Moveable);
	}

	

	////Append Child Nodes for DevorationsNode
	g_pDecorations->AddChildNode(g_pGround);
	g_pDecorations->AddChildNode(g_pEndNode);

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

	
	Gravity();
	ClearBackBuffer();
	/*SetLighting();
	SetVertexBuffer();
	SetWorldMatrix();
	SetLightWorldMatrix();
	SetConstantBuffer();
	SetWorldViewProjection();*/

	// Select which primitive type to use //03-01
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Collectables Logic
	for (int i = 0;i < ARRAYSIZE(g_pCollectables); i++)
	{
		g_pCollectable->Logic(g_pCollectables[i], g_pRootNode, g_pPlayer);
	}

	//Enemy Logic
	//...

	////Camera check
	//if ((g_pCamera->GetX() - g_pPlayer->GetX()) > 0.2f || (g_pCamera->GetX() - g_pPlayer->GetX()) > -0.2f
	//	|| (g_pCamera->GetY() - g_pPlayer->GetY()) > 0.2f || (g_pCamera->GetY() - g_pPlayer->GetY()) > -0.2f
	//	|| (g_pCamera->GetZ() - g_pPlayer->GetZ()) > 0.2f || (g_pCamera->GetZ() - g_pPlayer->GetZ()) > -0.2f)
	//{
	//	g_pCamera->SetX(g_pPlayer->GetX());
	//	g_pCamera->SetY(g_pPlayer->GetY());
	//	g_pCamera->SetZ(g_pPlayer->GetZ());
	//}
	
	g_pInput->KeyLogic(g_pCamera, g_pRootNode, g_pPlayer, g_pCollideable);
	g_pInput->MouseLogic(g_pCamera, g_pPlayer, g_pRootNode);
	FrameLimit();
	Draw();

	

	//render all the stuff
	g_pSwapChain->Present(0, 0);
}

void Renderer::Gravity()
{
	//apply gravity to vertical velocity if player is over ground
	if(g_pPlayer->GetY() > 0)
		g_pCamera->AddVelocityY(g_gravity);


	//only call MoveUp function when velocity is not zero
	if (g_pCamera->GetVelocityY() != 0.0f)
	{
		if (!g_pPlayer->MoveUp(g_pCamera->GetVelocityY(), g_pRootNode, g_pCollideable, g_pPlayer, true))
		{
			g_pCamera->MoveUp();

			XMMATRIX identity = XMMatrixIdentity();
			g_pRootNode->UpdateCollisionTree(&identity, 1.0f, g_pPlayer);
		}
		else
		{
			g_pInput->SetPressed(false);
			g_pCamera->SetVelocityY(0.0f);
		}
	}
	
	

	//if player is on the ground set vertical velocity to zero
	if (g_pPlayer->GetY() <= 0.0f)
	{
		g_pInput->SetPressed(false);
		g_pCamera->SetVelocityY(0.0f);

		g_pPlayer->SetPosY(0.0f);
		g_pCamera->SetY(0.0f);

		XMMATRIX identity = XMMatrixIdentity();
		g_pRootNode->UpdateCollisionTree(&identity, 1.0f, g_pPlayer);
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


	if (g_pTextureGrass) g_pTextureGrass->Release();
	if (g_pTextureTile) g_pTextureTile->Release();
	if (g_pTextureWall) g_pTextureWall->Release();
	if (g_pModelBlock) delete g_pModelBlock;
	if (g_pModelCoin) delete g_pModelCoin;
	if (g_pModelGate) delete g_pModelGate;
	if (g_pModelPyramid) delete g_pModelPyramid;
	if (g_pModelPlane) delete g_pModelPlane;
	if (g_pModelCube) delete g_pModelCube;
	if (g_pModelSphere) delete g_pModelSphere;
	if (g_pLight) delete g_pLight;

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