#include "camera.h"

Camera::Camera(float X, float Y, float Z, float RotationY)
{
	m_x = X;
	m_y = Y;
	m_z = Z;
	m_camera_rotationY = RotationY;

	m_dx = sin(m_camera_rotationY * (XM_PI / 180.0f));
	m_dz = cos(m_camera_rotationY * (XM_PI / 180.0f));
}

Camera::~Camera()
{

}

void Camera::Rotate(float Amount)
{
	m_camera_rotationY += Amount;

	m_dx = sin(m_camera_rotationY * (XM_PI / 180.0f));
	m_dz = cos(m_camera_rotationY * (XM_PI / 180.0f));
}

void Camera::Forward(float Distance)
{
	m_x += m_dx * Distance;
	m_z += m_dz * Distance;
}

void Camera::MoveUp()
{
	m_y += m_velocityY;
}

void Camera::Strafe(float Distance)
{
	XMVECTOR right = XMVector3Cross((m_position - m_lookat), (m_up - m_position));
	m_x += right.m128_f32[0] * Distance;
	m_z += right.m128_f32[2] * Distance;
}

void Camera::AddVelocityY(float Value)
{
	m_velocityY += Value;
}

//__________________________________________________SETTER
void Camera::SetX(float value) { m_x = value; }
void Camera::SetY(float Value) { m_y = Value; }
void Camera::SetZ(float value) { m_x = value; }

void Camera::SetVelocityY(float Value)
{
	m_velocityY = Value;
}



//__________________________________________________GETTER
XMMATRIX Camera::GetViewMatrix()
{
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	XMMATRIX view = XMMatrixLookAtLH(m_position, m_lookat, m_up);

	return view;
}

float Camera::GetVelocityY() { return m_velocityY; }
float Camera::GetX() { return m_x; }
float Camera::GetY() { return m_y; }
float Camera::GetZ() { return m_z; }
float Camera::GetLookAtX() { return m_dx; }
float Camera::GetLookAtY() { return m_dy; }
float Camera::GetLookAtZ() { return m_dz; }
