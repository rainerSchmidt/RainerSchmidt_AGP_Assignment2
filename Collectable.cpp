#include "collectable.h"

Collectable::Collectable()
{
	//m_frameCount = 0;
}

Collectable::~Collectable()
{

}

void Collectable::Logic(SceneNode* Node, SceneNode* RootNode, SceneNode* Player)
{
	Node->Rotate(3.0f, RootNode, Player);

	////jump up and down depended on frame
	//if (m_frameCount < 60.0f)
	//{
	//	Node->MoveUp(0.3f - (m_frameCount/200.0f) , RootNode, NULL, Player, false);
	//}
	//else
	//{
	//	Node->MoveUp(((m_frameCount / 200.0f) - 0.3f)*-1, RootNode, NULL, Player, false);
	//}

	//if (m_frameCount >= 120.0f)
	//	m_frameCount = 0.0f;

	//m_frameCount += 1.0f;
}