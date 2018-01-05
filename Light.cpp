#include "lighting.h"

//Constructor
Light::Light()
{

}

//Destructor
Light::~Light()
{

}

//Initialises light variables that are used by other classes
void Light::InitialiseLighting()
{
	m_pDirectionalLightShinesFrom = { -2.0f, 2.0f, 10.0f, 0.0f };
	m_pDirectionalLightColor = { 1.0f, 1.0f, 1.0f, 0.0f };
	m_pAmbientLightColor = { 0.2f, 0.2f, 0.2f, 0.0f };
}

//__________________GETTER
XMFLOAT4 Light::GetAmbientLightColor() { return m_pAmbientLightColor; }
XMFLOAT4 Light::GetDirectionalLightColor() { return m_pDirectionalLightColor; }
XMFLOAT4 Light::GetDirectionalLightShinesFrom() { return m_pDirectionalLightShinesFrom; }