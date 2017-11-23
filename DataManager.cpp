#include "datamanager.h";

DataManager::DataManager()
{
	//create object instances
	g_pRenderer = new Renderer();
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