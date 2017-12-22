#include "player.h"

Player::Player(float PositionX, float PositionY, float PositionZ, float Rotation)
{
	m_x = PositionX;
	m_y = PositionY;
	m_z = PositionZ;
	m_rotation = Rotation;

	m_dx = sin(m_rotation * (XM_PI / 180.0));
	m_dz = cos(m_rotation * (XM_PI / 180.0));
}

Player::~Player()
{

}

void Player::Rotate(float AmountInDegrees)
{
	m_rotation += AmountInDegrees;

	m_dx = sin(m_rotation * (XM_PI / 180.0));
	m_dz = cos(m_rotation * (XM_PI / 180.0));
}

void Player::Forward(float Distance)
{
	m_x += m_dx * Distance;
	m_z += m_dz * Distance;
}

void Player::Up()
{

}

void Player::Strafe(float Distance)
{
	m_dx += Distance;
	Rotate(-1*(atan(m_dz / m_dx) * (XM_PI * 180)));
}

XMMATRIX Player::GetViewMatrix()
{
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	XMMATRIX view = XMMatrixLookAtLH(m_position, m_lookat, m_up);

	return view;
}