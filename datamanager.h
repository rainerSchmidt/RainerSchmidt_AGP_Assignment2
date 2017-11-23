#pragma once
#include "renderer.h"

class DataManager
{
	public:
		DataManager();
		~DataManager();
		void Render();
	private:
		Renderer* g_pRenderer;
		void CleanUp();
};