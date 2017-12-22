#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;

class Player
{
public:
	Player(float PositionX, float PositionY, float PositionZ, float Rotation);
	~Player();
	void Rotate(float AmountInDegrees);
	void Forward(float distance);
	void Up();
	void Strafe(float Distance);
	XMMATRIX GetViewMatrix();
private:
	float m_x, m_y, m_z, m_dx, m_dz, m_rotation = 0.0f;
	XMVECTOR m_position, m_lookat, m_up;

};
