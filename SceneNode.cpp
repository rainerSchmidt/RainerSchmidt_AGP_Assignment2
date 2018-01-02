#include "scenenode.h"

SceneNode::SceneNode(float X, float Y, float Z, float RotX, float RotY, float RotZ, float Scale)
{
	m_pModel = NULL;

	m_x = X;
	m_y = Y;
	m_z = Z;
	m_xangle = RotX;
	m_yangle = RotY;
	m_zangle = RotZ;
	m_scale = Scale;
}

void SceneNode::AddChildNode(SceneNode* Node)
{
	m_children.push_back(Node);
}

bool SceneNode::DetatchNode(SceneNode* Node)
{
	//traverse tree to find node to detatch
	for (int i = 0; i < m_children.size(); i++)
	{
		if (Node == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}
		if (m_children[i]->DetatchNode(Node) == true) return true;
	}

	return false; //note not in this tree
}

void SceneNode::Execute(XMMATRIX* World, XMMATRIX* View, XMMATRIX* Projection)
{
	//the local_world_matrix will be used to calc the local transformations fir this node
	XMMATRIX local_world = XMMatrixIdentity();

	local_world = XMMatrixScaling(m_scale, m_scale, m_scale);
	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));
	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	//the local matrix is multiplied by the passed in world matrix that contains the concatenated
	//transformations of all parent nodes so that this nodes transformations are relative to those
	local_world *= *World;

	//only draw if there is a model attached
	if (m_pModel) m_pModel->Draw(&local_world, View, Projection);

	//traverse all child nodes, passing in the concatenated world matrix
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Execute(&local_world, View, Projection);
	}	
}

void SceneNode::LookAt_XZ(float X, float Z)
{
	float DX = X - m_x;
	float DZ = Z - m_z;

	m_yangle = atan2(DX, DZ) * (180.0 / XM_PI);
}

bool SceneNode::MoveForwards(float Distance, SceneNode* RootNode, bool CollisionCheck)
{
	float old_x = m_x;
	float old_z = m_z;

	m_x += sin(m_yangle * (XM_PI / 180.0)) * Distance;
	m_z += cos(m_yangle * (XM_PI / 180.0)) * Distance;

	XMMATRIX Identity = XMMatrixIdentity();

	//since state has changed, need to update collision tree
	//this basic system requires entire hierarchy to be updated
	//so start at root_node passing in identity matrix
	RootNode->UpdateCollisionTree(&Identity, 1.0);
	if (CollisionCheck)
	{
		if (CheckCollision(RootNode) == true)
		{
			// if collision restore state
			m_x = old_x;
			m_z = old_z;

			return true;
		}
	}
	//if (CheckCollision(RootNode) == true)
	//{
	//	// if collision restore state
	//	m_x = old_x;
	//	m_z = old_z;

	//	return true;
	//}

	return false;
}

bool SceneNode::MoveUp(float Distance, SceneNode* RootNode, bool CollisionCheck)
{
	float old_y = m_y;
	m_y += Distance;

	XMMATRIX Identity = XMMatrixIdentity();

	//since state has changed, need to update collision tree
	//this basic system requires entire hierarchy to be updated
	//so start at root_node passing in identity matrix
	RootNode->UpdateCollisionTree(&Identity, 1.0);
	if (CollisionCheck)
	{
		if (CheckCollision(RootNode) == true)
		{
			// if collision restore state
			m_y = old_y;

			return true;
		}
	}
	//if (CheckCollision(RootNode) == true)
	//{
	//	// if collision restore state
	//	m_y = old_y;

	//	return true;
	//}

	return false;
}

bool SceneNode::Strafe(float Distance, SceneNode* RootNode, bool CollisionCheck)
{
	float old_x = m_x;
	float old_z = m_z;

	XMVECTOR position = XMVectorSet(m_x, m_y, m_z, 0.0f);
	XMVECTOR lookat = XMVectorSet(m_x + sin(m_yangle * (XM_PI / 180.0f)), m_y, m_z + cos(m_yangle * (XM_PI / 180.0f)), 0.0);
	XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	XMVECTOR right = XMVector3Cross((position - lookat), (up - position));
	m_x += right.vector4_f32[0] * Distance;
	m_z += right.vector4_f32[2] * Distance;

	XMMATRIX Identity = XMMatrixIdentity();

	//since state has changed, need to update collision tree
	//this basic system requires entire hierarchy to be updated
	//so start at root_node passing in identity matrix
	RootNode->UpdateCollisionTree(&Identity, 1.0);
	if (CollisionCheck)
	{
		if (CheckCollision(RootNode) == true)
		{
			// if collision restore state
			m_x = old_x;
			m_z = old_z;

			return true;
		}
	}
	//if (CheckCollision(RootNode) == true)
	//{
	//	// if collision restore state
	//	m_x = old_x;
	//	m_z = old_z;

	//	return true;
	//}

	return false;

}

void SceneNode::Rotate(float Amount, SceneNode* RootNode)
{
	m_yangle += Amount;
	XMMATRIX Identity = XMMatrixIdentity();

	//since state has changed, need to update collision tree
	//this basic system requires entire hierarchy to be updated
	//so start at root_node passing in identity matrix
	RootNode->UpdateCollisionTree(&Identity, 1.0);
}

bool SceneNode::CheckCollision(SceneNode* CompareTree)
{
	return CheckCollision(CompareTree, this);
}

bool SceneNode::CheckCollision(SceneNode* CompareTree, SceneNode* ObjectTreeRoot)
{
	// check to see if root of tree being compared is same as root node of object tree being checked
	// i.e. stop pbject node and children being checked against eachother
	if (ObjectTreeRoot == CompareTree) return false;

	// only check for collisions if both nodes contain a model
	if (m_pModel && CompareTree->m_pModel)
	{
		XMVECTOR v1 = GetWorldCenterPosition();
		XMVECTOR v2 = CompareTree->GetWorldCenterPosition();
		XMVECTOR vdiff = v1 - v2;

		float x1 = XMVectorGetX(v1);
		float y1 = XMVectorGetY(v1);
		float z1 = XMVectorGetZ(v1);
		float x2 = XMVectorGetX(v2);
		float y2 = XMVectorGetY(v2);
		float z2 = XMVectorGetZ(v2);

		float dx = x1 - x2;
		float dy = y1 - y2;
		float dz = z1 - z2;

		// check bounding sphere collision
		if (sqrt(dx*dx + dy*dy + dz*dz) <
			(CompareTree->m_pModel->GetBoundingSphereRadius() * CompareTree->m_world_scale) +
			(this->m_pModel->GetBoundingSphereRadius() * m_world_scale))
		{
			return true;
		}
	}

	// iterate through compared tree child nodes
	for (int i = 0; i < CompareTree->m_children.size(); i++)
	{
		//check for collision against all compared tree child nodes
		if (CheckCollision(CompareTree->m_children[i], ObjectTreeRoot) == true) return true;
	}

	// iterate through composite object child nodes
	for (int i = 0; i < m_children.size(); i++)
	{
		// check all the child nodes of the composite object against compared tree
		if (m_children[i]->CheckCollision(CompareTree, ObjectTreeRoot) == true) return true;
	}

	return false;
}

void SceneNode::UpdateCollisionTree(XMMATRIX* World, float Scale)
{

	m_local_world_matrix = XMMatrixIdentity();

	m_local_world_matrix = XMMatrixScaling(m_scale, m_scale, m_scale);
	m_local_world_matrix *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	m_local_world_matrix *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	m_local_world_matrix *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));
	m_local_world_matrix *= XMMatrixTranslation(m_x, m_y, m_z);
	
	m_local_world_matrix *= *World;
	m_world_scale = Scale * m_scale;

	XMVECTOR v;
	if (m_pModel)
	{
		v = XMVectorSet(m_pModel->GetBoundingSphereCentreX(),
						m_pModel->GetBoundingSphereCentreY(),
						m_pModel->GetBoundingSphereCentreZ(), 0.0);
	}
	else
	{
		v = XMVectorSet(0, 0, 0, 0); // no Model, default to 0
	}

	// find and store world space bounding sphere center
	v = XMVector3Transform(v, m_local_world_matrix);
	m_world_centerX = XMVectorGetX(v);
	m_world_centerY = XMVectorGetY(v);
	m_world_centerZ = XMVectorGetZ(v);

	// traverse all childe nodes, passing in the concatenated world matrix and scale
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateCollisionTree(&m_local_world_matrix, m_world_scale);
	}

}

bool SceneNode::CheckCollisionRay(SceneNode* Node, float DirPosX, float DirPosY, float DirPosZ)
{
	////the local_world_matrix will be used to calc the local transformations fir this node
	//XMMATRIX local_world = XMMatrixIdentity();

	//local_world = XMMatrixScaling(m_scale, m_scale, m_scale);
	//local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	//local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	//local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));
	//local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	////the local matrix is multiplied by the passed in world matrix that contains the concatenated
	////transformations of all parent nodes so that this nodes transformations are relative to those
	//local_world *= *World;

	////only draw if there is a model attached
	//if (m_pModel) m_pModel->Draw(&local_world, View, Projection);

	////traverse all child nodes, passing in the concatenated world matrix
	//for (int i = 0; i < m_children.size(); i++)
	//{
	//	m_children[i]->Execute(&local_world, View, Projection);
	//}

	XMVECTOR ray = XMVectorSet(DirPosX, DirPosY, DirPosZ, 0.0);

	//ray = XMVector3Transform(ray, m_local_world_matrix);
	XMVECTOR rayStart = XMVectorSet(Node->GetX(), Node->GetY(), Node->GetZ(), 0.0);
	//XMVECTOR rayStart = XMVectorSet(RayPosX, RayPosY, RayPosZ, 0.0);

	ObjFileModel::xyz* vectorRayStart = new ObjFileModel::xyz;
	vectorRayStart->x = rayStart.vector4_f32[0];
	vectorRayStart->y = rayStart.vector4_f32[1];
	vectorRayStart->z = rayStart.vector4_f32[2];

	ObjFileModel::xyz* vectorRayEnd = new ObjFileModel::xyz;
	vectorRayEnd->x = rayStart.vector4_f32[0] + ray.vector4_f32[0];
	vectorRayEnd->y = rayStart.vector4_f32[1] + ray.vector4_f32[1];
	vectorRayEnd->z = rayStart.vector4_f32[2] + ray.vector4_f32[2];

	/*ObjFileModel::xyz* vectorRayEnd = new ObjFileModel::xyz;
	vectorRayEnd->x = rayStart.vector4_f32[0] + DirPosX;
	vectorRayEnd->y = rayStart.vector4_f32[1] + DirPosY;
	vectorRayEnd->z = rayStart.vector4_f32[2] + DirPosZ;*/


	if (m_pModel)
	{
		/*float distanceRayToNode = sqrt(((m_x - RayPosX)*(m_x - RayPosX)) +
									((m_y - RayPosY)*(m_y - RayPosY)) +
									((m_z - RayPosZ)*(m_z - RayPosZ)));*/

		float distanceRayToNode = sqrt(((m_x - Node->GetX())*(m_x - Node->GetX())) +
			((m_y - Node->GetY())*(m_y - Node->GetY())) +
			((m_z - Node->GetZ())*(m_z - Node->GetZ())));

		float rayLength = sqrt((ray.vector4_f32[0] * ray.vector4_f32[0]) +
							(ray.vector4_f32[1] * ray.vector4_f32[1]) +
							(ray.vector4_f32[2] * ray.vector4_f32[2]));

		/*float rayLength = sqrt((DirPosX * DirPosX) +
								(DirPosY * DirPosY) +
								(DirPosZ * DirPosZ));*/

		float sum = m_pModel->GetBoundingSphereRadius() + rayLength;

		

		if (distanceRayToNode < sum)
		{
			ObjFileModel* pObject = m_pModel->GetModelObject();

			for (int i = 0; i < pObject->numverts; i+=3)
			{
				//set vertex data
				XMVECTOR p1 = XMVectorSet(pObject->vertices[i].Pos.x,
										pObject->vertices[i].Pos.y,
										pObject->vertices[i].Pos.z, 0.0f);

				XMVECTOR p2 = XMVectorSet(pObject->vertices[i+1].Pos.x,
										pObject->vertices[i + 1].Pos.y,
										pObject->vertices[i + 1].Pos.z, 0.0f);

				XMVECTOR p3 = XMVectorSet(pObject->vertices[i+2].Pos.x,
										pObject->vertices[i + 2].Pos.y,
										pObject->vertices[i + 2].Pos.z, 0.0f);

				//transform the vertex data
				p1 = XMVector3Transform(p1, m_local_world_matrix);
				p2 = XMVector3Transform(p2, m_local_world_matrix);
				p3 = XMVector3Transform(p3, m_local_world_matrix);

				//convert vectors to xyz struct
				ObjFileModel::xyz *vectorP1 = new ObjFileModel::xyz;
				vectorP1->x = p1.vector4_f32[0];
				vectorP1->y = p1.vector4_f32[1];
				vectorP1->z = p1.vector4_f32[2];

				ObjFileModel::xyz *vectorP2 = new ObjFileModel::xyz;
				vectorP2->x = p2.vector4_f32[0];
				vectorP2->y = p2.vector4_f32[1];
				vectorP2->z = p2.vector4_f32[2];

				ObjFileModel::xyz *vectorP3 = new ObjFileModel::xyz;
				vectorP3->x = p3.vector4_f32[0];
				vectorP3->y = p3.vector4_f32[1];
				vectorP3->z = p3.vector4_f32[2];

				//calculate plane through plane equation
				Maths::Plane plane = Maths::PlaneEquation(vectorP1, vectorP2, vectorP3);

				//get intersection point if there is any
				ObjFileModel::xyz* intersectionPoint = new ObjFileModel::xyz;
				*intersectionPoint = Maths::PlaneIntersection(&plane, vectorRayStart, vectorRayEnd);

				if (intersectionPoint->x != 0 || intersectionPoint->y != 0 || intersectionPoint->z != 0)
				{	//Intersection point was calculated
					//check if intersection point is in the current triangle
					if(Maths::InTriangle(vectorP1, vectorP2, vectorP3, intersectionPoint))
						return true; //collsion happend and intersection point lies inside the current triangle
				}
				// no collision was detected in this triangle, continue with next triangle
			}
		}
	}

	//traverse all child nodes, passing in the concatenated world matrix
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->CheckCollisionRay(Node, DirPosX, DirPosY, DirPosZ))
			return true;
	}
	

	//no collision with this or any of the child nodes
	return false;
}

//SETTER
void SceneNode::SetPosX(float X) { m_x = X; }
void SceneNode::SetPosY(float Y) { m_y = Y; }
void SceneNode::SetPosZ(float Z) { m_z = Z; }
void SceneNode::SetRotX(float Amount) { m_xangle = Amount; }
void SceneNode::AddRotX(float Amount) { m_xangle += Amount; }
void SceneNode::SetRotY(float Amount) { m_yangle = Amount; }
void SceneNode::SetRotZ(float Amount) { m_zangle = Amount; }
//void SceneNode::SetScale(float Amount) { m_scale = Amount; }
void SceneNode::SetModel(Model* Model) { m_pModel = Model; }

//GETTER
Model* SceneNode::GetModel() { return m_pModel; };
XMVECTOR SceneNode::GetWorldCenterPosition()
{
	return XMVectorSet(m_world_centerX, m_world_centerY, m_world_centerZ,0.0);
}

float SceneNode::GetX() { return m_x; }
float SceneNode::GetY() { return m_y; }
float SceneNode::GetZ() { return m_z; }
float SceneNode::GetRotX() { return m_xangle; }
float SceneNode::GetRotY() { return m_yangle; }
float SceneNode::GetRotZ() { return m_zangle; }