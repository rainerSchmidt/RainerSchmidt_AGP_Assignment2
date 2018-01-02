#pragma once
#include "model.h"
#define _XM_NO_INTINSICS_
#define XM_NO_ALIGNMENT
//#include <xnamath.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "maths.h"

using namespace DirectX;

class SceneNode
{
private:
	Model*				m_pModel;
	vector<SceneNode*>	m_children;
	XMMATRIX			m_local_world_matrix;

	float m_x, m_y, m_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scaleX, m_scaleY, m_scaleZ;
	float m_world_centerX, m_world_centerY, m_world_centerZ;
	float m_world_scale;

public:
	SceneNode(float X, float Y, float Z, float RotX, float RotY, float RotZ, float ScaleX, float ScaleY, float ScaleZ);
	~SceneNode();

	void LookAt_XZ(float X, float Z);
	void Rotate(float Amount, SceneNode* RootNode);
	bool MoveForwards(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, bool CollisionCheck);
	bool MoveUp(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, bool CollisionCheck);
	bool Strafe(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, bool CollisionCheck);
	bool CheckCollision(SceneNode* CompareTree);
	bool CheckCollision(SceneNode* CompareTree, SceneNode* ObjectTreeRoot);
	void UpdateCollisionTree(XMMATRIX* World, float Scale);
	bool CheckCollisionRay(SceneNode* Node, float DirPosX, float DirPosY, float DirPosZ);

	float GetX();
	float GetY();
	float GetZ();
	float GetRotX();
	float GetRotY();
	float GetRotZ();
	float GetScaleX();
	float GetScaleY();
	float GetScaleZ();
	Model* GetModel();
	XMVECTOR GetWorldCenterPosition();

	void SetPosX(float pos);
	void SetPosY(float pos);
	void SetPosZ(float pos);
	void SetRotX(float rot);
	void AddRotX(float rot);
	void SetRotY(float rot);
	void SetRotZ(float rot);
	void SetScale(float amount);
	void SetModel(Model* Model);

	void AddChildNode(SceneNode* Node);
	bool DetatchNode(SceneNode* Node);
	void Execute(XMMATRIX* World, XMMATRIX* View, XMMATRIX* Projection);
};