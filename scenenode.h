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

public:

	enum Tag { Player, Enemy, Collectable, Block, Other };

	SceneNode(float X, float Y, float Z, float RotX, float RotY, float RotZ, float ScaleX, float ScaleY, float ScaleZ);
	~SceneNode();

	
	void LookAt_XZ(float X, float Z);
	void Rotate(float Amount, SceneNode* RootNode, SceneNode* Player);
	bool MoveForwards(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, SceneNode* Player, bool CollisionCheck);
	bool MoveUp(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, SceneNode*, bool CollisionCheck);
	bool Strafe(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, SceneNode* Player, bool CollisionCheck);
	bool CheckCollision(SceneNode* CompareTree);
	bool CheckCollision(SceneNode* CompareTree, SceneNode* ObjectTreeRoot);
	void UpdateCollisionTree(XMMATRIX* World, float Scale, SceneNode* Player);
	bool CheckCollisionRay(SceneNode* Node, float DirPosX, float DirPosY, float DirPosZ);
	void CheckIfCloseToPlayer(SceneNode* Player);

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
	Tag GetNodeTag();

	void SetPosX(float pos);
	void SetPosY(float pos);
	void SetPosZ(float pos);
	void SetRotX(float rot);
	void AddRotX(float rot);
	void SetRotY(float rot);
	void SetRotZ(float rot);
	void SetScale(float amount);
	void SetModel(Model* Model);
	void SetTag(Tag NodeTag);

	void AddChildNode(SceneNode* Node);
	bool DetatchNode(SceneNode* Node);
	void Execute(XMMATRIX* World, XMMATRIX* View, XMMATRIX* Projection);

private:
	Model*				m_pModel;
	vector<SceneNode*>	m_children;
	XMMATRIX			m_local_world_matrix;

	float m_x, m_y, m_z;
	float m_xangle, m_zangle, m_yangle;
	float m_scaleX, m_scaleY, m_scaleZ;
	float m_world_centerX, m_world_centerY, m_world_centerZ;
	float m_world_scale;
	bool m_IsCollideable = false;
	Tag m_tag;
};