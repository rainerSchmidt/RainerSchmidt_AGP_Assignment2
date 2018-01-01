#pragma once
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;

class Camera
{
public:
	Camera(float X, float Y, float Z, float RotationY);
	~Camera();

	void Rotate(float Amount);
	void Forward(float Distance);
	void MoveUp();
	void Strafe(float Distance);

	float GetX();
	float GetY();
	float GetZ();
	float GetLookAtX();
	float GetLookAtY();
	float GetLookAtZ();
	float GetVelocityY();
	void AddVelocityY(float Value);
	void SetVelocityY(float Value);
	void SetY(float Value);

	XMMATRIX GetViewMatrix();

private:
	float m_x, m_y, m_z;
	float m_dx, m_dy, m_dz;
	float m_camera_rotationY = 0.0f;
	float m_velocityY = 0.0f;
	XMVECTOR m_position, m_lookat, m_up;
};
