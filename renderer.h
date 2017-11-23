#pragma once

class Renderer
{
	public:
		void RenderFrame();
		~Renderer();
	private:
		void CleanUp();
		void ClearBackBuffer();
		void SetLighting();
		void SetVertexBuffer();
		void SetWorldMatrix();
		void SetLightWorldMatrix();
		void SetConstantBuffer();
		void SetWorldViewProjection();
		void SetContext();
		void Draw();
};


