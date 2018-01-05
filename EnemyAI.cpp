#include "enemyai.h"

EnemyAI::EnemyAI()
{
	
}

EnemyAI::~EnemyAI()
{

}

void EnemyAI::SetWayPoints(SceneNode* P1, SceneNode* P2, SceneNode* P3)
{
	m_waypoints[0] = P1;
	m_waypoints[1] = P2;
	m_waypoints[2] = P3;
}

void EnemyAI::Logic(SceneNode* Node, SceneNode* RootNode, SceneNode* Collideables, SceneNode* Player)
{
	// Calculate distance to the player
	float playerDistX = Player->GetX() - Node->GetX();
	float playerDistZ = Player->GetZ() - Node->GetZ();

	if ((playerDistX < 5.0f && playerDistX > -5.0f) && (playerDistZ < 5.0f && playerDistZ > -5.0f))
		m_state = Chase;
	else
		m_state = Patrol;

	float distanceX = 0.0f;
	float distanceZ = 0.0f;

	switch (m_state)
	{
	case EnemyAI::Patrol:

		//Get Distance between waypoint and enemy node
		distanceX = m_waypoints[m_waypointCounter]->GetX() - Node->GetX();
		distanceZ = m_waypoints[m_waypointCounter]->GetZ() - Node->GetZ();


		if ((distanceX < 1.0f && distanceX > -1.0f) && (distanceZ < 1.0f && distanceZ > -1.0f))
		{
			//count up the waypoint Counter
			m_waypointCounter = m_waypointCounter >= 2 ? 0 : m_waypointCounter + 1;
		}
		else
		{
			//move towards next waypoint
			Node->LookAt_XZ(m_waypoints[m_waypointCounter]->GetX(), m_waypoints[m_waypointCounter]->GetZ());
			Node->MoveForwards(m_speed, RootNode, Collideables, Player, false);
			
			//if (distanceX > distanceZ)
			//{
			//	//Node->Strafe((m_speed > distanceX ? distanceX : m_speed), RootNode, Collideables, Player, false);
			//	
			//}
			//else
			//{
			//	Node->MoveForwards((m_speed > distanceZ ? distanceZ : m_speed), RootNode, Collideables, Player, false);
			//}
		}

		break;

	case EnemyAI::Chase:

		//enemy sees player
		Node->LookAt_XZ(Player->GetX(), Player->GetZ());
		Node->MoveForwards(m_speed, RootNode, Collideables, Player, false);

		break;
	default:
		break;
	}
}