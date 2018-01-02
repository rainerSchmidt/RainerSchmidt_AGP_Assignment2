#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;

class Light
{
private:

	XMFLOAT4 m_pDirectionalLightColor;
	XMFLOAT4 m_pDirectionalLightShinesFrom;
	XMFLOAT4 m_pAmbientLightColor;

public:

	Light();
	~Light();

	XMFLOAT4 GetDirectionalLightColor();
	XMFLOAT4 GetDirectionalLightShinesFrom();
	XMFLOAT4 GetAmbientLightColor();
	void InitialiseLighting();
};
