#include "datamanager.h";

DataManager::DataManager(ID3D11Device* D3DDevice, ID3D11DeviceContext* DeviceContext, IDXGISwapChain* SwapChain, ID3D11RenderTargetView* BackBuffer,
						ID3D11Buffer* VertexBuffer, ID3D11Buffer* TransformationBuffer, ID3D11Buffer* LightBuffer, ID3D11DepthStencilView* ZBuffer,
						ID3D11ShaderResourceView* Texture, ID3D11SamplerState* Sampler ,Player* Player//, XMVECTOR DirectionalLightDirection, XMVECTOR DirectionalLightColor, XMVECTOR AmbientLightColor);
)
{
	//create object instances
	g_pRenderer = new Renderer(D3DDevice, DeviceContext, SwapChain, BackBuffer, VertexBuffer, TransformationBuffer, LightBuffer, ZBuffer, Texture, Sampler, Player);
}

DataManager::~DataManager()
{
	//call CleanUp function
	CleanUp();
}

void DataManager::CleanUp()
{
	//release objects
	g_pRenderer->~Renderer();
}

void DataManager::Render()
{	
	//render
	g_pRenderer->RenderFrame();
}