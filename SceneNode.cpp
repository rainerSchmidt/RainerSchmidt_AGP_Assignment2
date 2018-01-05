#include "scenenode.h"

//Constructor: sets member with parameter
SceneNode::SceneNode(float X, float Y, float Z, float RotX, float RotY, float RotZ, float ScaleX, float ScaleY, float ScaleZ)
{
	m_pModel = NULL;

	m_x = X;
	m_y = Y;
	m_z = Z;
	m_xangle = RotX;
	m_yangle = RotY;
	m_zangle = RotZ;
	m_scaleX = ScaleX;
	m_scaleY = ScaleY;
	m_scaleZ = ScaleZ;
}

//Adds a child node to this node
void SceneNode::AddChildNode(SceneNode* Node)
{
	m_children.push_back(Node);
}

//Detaches the given node from this node
//Not used, planned to use for animations
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

//updates local world matrix and draws objects attached
//goes recursively through each child node
void SceneNode::Execute(XMMATRIX* World, XMMATRIX* View, XMMATRIX* Projection)
{
	//the local_world_matrix will be used to calc the local transformations fir this node
	XMMATRIX local_world = XMMatrixIdentity();

	local_world = XMMatrixScaling(m_scaleX, m_scaleY, m_scaleZ);
	local_world *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	local_world *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	local_world *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));
	local_world *= XMMatrixTranslation(m_x, m_y, m_z);

	//the local matrix is multiplied by the passed in world matrix that contains the concatenated
	//transformations of all parent nodes so that this nodes transformations are relative to those
	local_world *= *World;

	//only draw if there is a model attached and the object is supposed to be drawn
	if (m_pModel && m_IsDrawn) m_pModel->Draw(&local_world, View, Projection);

	//traverse all child nodes, passing in the concatenated world matrix
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Execute(&local_world, View, Projection);
	}	
}

//lets the node face towards the given coordinates
void SceneNode::LookAt_XZ(float X, float Z)
{
	float DX = X - m_x;
	float DZ = Z - m_z;

	m_yangle = atan2(DX, DZ) * (180.0 / XM_PI);
}

//moves the node forward by the given amount and updates collision tree
//restores old position if collision occured
bool SceneNode::MoveForwards(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, SceneNode* Player, bool CollisionCheck)
{
	float old_x = m_x;
	float old_z = m_z;

	m_x += sin(m_yangle * (XM_PI / 180.0)) * Distance;
	m_z += cos(m_yangle * (XM_PI / 180.0)) * Distance;

	XMMATRIX Identity = XMMatrixIdentity();

	RootNode->UpdateCollisionTree(&Identity, 1.0, Player);
	if (CollisionCheck)
	{
		if (CollisionObjects->CheckCollisionRay(this, m_x, m_y, m_z, RootNode) == true)
		{
			// if collision restore state
			m_x = old_x;
			m_z = old_z;

			return true;
		}
	}

	return false;
}

//moves the node in y-axis and updates the collision tree
//restores old position if collision would occur
bool SceneNode::MoveUp(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, SceneNode* Player, bool CollisionCheck)
{
	float old_y = m_y;
	m_y += Distance;

	XMMATRIX Identity = XMMatrixIdentity();

	RootNode->UpdateCollisionTree(&Identity, 1.0, Player);
	if (CollisionCheck)
	{
		if (CollisionObjects->CheckCollisionRay(this, m_x, m_y, m_z, RootNode) == true)
		{
			// if collision restore state
			m_y = old_y;

			return true;
		}
	}

	return false;
}

//moves the node in x-axis and updates the collision tree
//restores old position if collision would occur
bool SceneNode::Strafe(float Distance, SceneNode* RootNode, SceneNode* CollisionObjects, SceneNode* Player, bool CollisionCheck)
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
	RootNode->UpdateCollisionTree(&Identity, 1.0, Player);
	if (CollisionCheck)
	{
		if (CollisionObjects->CheckCollisionRay(this, m_x, m_y, m_z, RootNode) == true)
		{
			// if collision restore state
			m_x = old_x;
			m_z = old_z;

			return true;
		}
	}

	return false;

}

//rotates the node by the given amount in y-axis and updates collision tree
void SceneNode::Rotate(float Amount, SceneNode* RootNode, SceneNode* Player)
{
	m_yangle += Amount;
	XMMATRIX Identity = XMMatrixIdentity();

	RootNode->UpdateCollisionTree(&Identity, 1.0, Player);
}

//only called by the rootnode, calles other checkcollision function
bool SceneNode::CheckCollision(SceneNode* CompareTree)
{
	return CheckCollision(CompareTree, this);
}

//Bounding sphere collision check, returns true if collision occured
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

//updates local world matrix for every object in the scene to ensure correct collision calculations
void SceneNode::UpdateCollisionTree(XMMATRIX* World, float Scale, SceneNode* Player)
{

	m_local_world_matrix = XMMatrixIdentity();

	m_local_world_matrix = XMMatrixScaling(m_scaleX, m_scaleY, m_scaleZ);
	m_local_world_matrix *= XMMatrixRotationX(XMConvertToRadians(m_xangle));
	m_local_world_matrix *= XMMatrixRotationY(XMConvertToRadians(m_yangle));
	m_local_world_matrix *= XMMatrixRotationZ(XMConvertToRadians(m_zangle));
	m_local_world_matrix *= XMMatrixTranslation(m_x, m_y, m_z);
	
	m_local_world_matrix *= *World;
	m_world_scale = (m_scaleX + m_scaleY + m_scaleZ) * Scale;
	//m_world_scale = Scale * m_scale;

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

	//check if object still needs to be calculated for collision
	CheckIfCloseToPlayer(Player);

	// traverse all childe nodes, passing in the concatenated world matrix and scale
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateCollisionTree(&m_local_world_matrix, m_world_scale, Player);
	}

}

//Collision check for every vertex of an object
//If collision, tag of the node determines action taken
bool SceneNode::CheckCollisionRay(SceneNode* Node, float DirPosX, float DirPosY, float DirPosZ, SceneNode* RootNode)
{
	XMVECTOR rayStart = XMVectorSet(Node->GetX(), Node->GetY(), Node->GetZ(), 0.0);

	ObjFileModel::xyz* vectorRayStart = new ObjFileModel::xyz;
	vectorRayStart->x = rayStart.vector4_f32[0];
	vectorRayStart->y = rayStart.vector4_f32[1];
	vectorRayStart->z = rayStart.vector4_f32[2];

	ObjFileModel::xyz* vectorRayEnd = new ObjFileModel::xyz;
	vectorRayEnd->x = rayStart.vector4_f32[0] + DirPosX;
	vectorRayEnd->y = rayStart.vector4_f32[1] + DirPosY;
	vectorRayEnd->z = rayStart.vector4_f32[2] + DirPosZ;


	if (m_pModel && m_IsCollideable)
	{
		float distanceRayToNode = sqrt(((m_x - Node->GetX())*(m_x - Node->GetX())) +
			((m_y - Node->GetY())*(m_y - Node->GetY())) +
			((m_z - Node->GetZ())*(m_z - Node->GetZ())));

		float rayLength = sqrt((DirPosX * DirPosX) +
								(DirPosY * DirPosY) +
								(DirPosZ * DirPosZ));

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
					if (Maths::InTriangle(vectorP1, vectorP2, vectorP3, intersectionPoint))
					{

						//Collision : check object tag to determine actions
						if(m_tag == Collectable) m_IsDrawn = false; //if object is an Collectable, no longer show the object

						if (m_tag == Moveable) //if object is moveable, move the object away from the player
						{
							//calculate difference in x- and z-axis between this Node and the Player
							float diffX = m_x - Node->GetX();
							float diffZ = m_z - Node->GetZ();
							bool signX = false;
							bool signZ = false;
							if (diffX < 0) { diffX *= -1; signX = true; }
							if (diffZ < 0) { diffZ *= -1; signZ = true; }
							
							//if z difference is higher move object in z direction, else in x direction
							if (diffX > diffZ)
							{
								this->MoveAway(signZ? -0.5 : 0.5, false, RootNode, Node);
							}
							else
							{
								this->MoveAway(signX ? -0.5 : 0.5, true, RootNode, Node);
							}
						}

						if (m_tag == Enemy)
						{
							//You have been hit by the enemy, the player is send back to the beginning;
							Node->MoveForwards(-1*(Node->GetZ()), RootNode, NULL, Node, false);
							Node->Strafe(-1 * (Node->GetX()), RootNode, NULL, Node, false);
						}

						if (m_tag == Goal)
						{
							// You won!!!
						}
							
						return true; //collsion happend and intersection point lies inside the current triangle
					}
						
				}
				// no collision was detected in this triangle, continue with next triangle
			}
		}
	}

	//traverse all child nodes, passing in the concatenated world matrix
	for (int i = 0; i < m_children.size(); i++)
	{
		if (m_children[i]->CheckCollisionRay(Node, DirPosX, DirPosY, DirPosZ, RootNode))
			return true;
	}
	

	//no collision with this or any of the child nodes
	return false;
}

//Moves the node away from the player
void SceneNode::MoveAway(float Distance, bool Direction, SceneNode* RootNode, SceneNode* Player)
{
	if (Direction)
	{
		m_x += sin(m_yangle * (XM_PI / 180.0)) * Distance;
		m_z += cos(m_yangle * (XM_PI / 180.0)) * Distance;

		XMMATRIX Identity = XMMatrixIdentity();
		RootNode->UpdateCollisionTree(&Identity, 1.0, Player);
	}
	else
	{
		XMVECTOR position = XMVectorSet(m_x, m_y, m_z, 0.0f);
		XMVECTOR lookat = XMVectorSet(m_x + sin(m_yangle * (XM_PI / 180.0f)), m_y, m_z + cos(m_yangle * (XM_PI / 180.0f)), 0.0);
		XMVECTOR up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
		XMVECTOR right = XMVector3Cross((position - lookat), (up - position));
		m_x += right.vector4_f32[0] * Distance;
		m_z += right.vector4_f32[2] * Distance;

		XMMATRIX Identity = XMMatrixIdentity();

		RootNode->UpdateCollisionTree(&Identity, 1.0, Player);
	}
}

//checks if node is close to player position
void SceneNode::CheckIfCloseToPlayer(SceneNode* Player)
{
	if (m_IsDrawn)// when the object is not drawn dont calculate collisions
	{
		if ((Player->GetZ() - m_z) > -2.0f && (Player->GetZ() - m_z) < 2.0f)
			m_IsCollideable = true;
		else
			m_IsCollideable = false;
	}
	else
		m_IsCollideable = false;
	
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
void SceneNode::SetTag(SceneNode::Tag NodeTag) { m_tag = NodeTag; }
void SceneNode::SetIsDrawn(bool Status) { m_IsDrawn = Status; }

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
SceneNode::Tag SceneNode::GetNodeTag() { return m_tag; }