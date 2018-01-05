#pragma once
#include "scenenode.h"

class EnemyAI
{
public:
	EnemyAI();
	~EnemyAI();
	void Logic(SceneNode* Node, SceneNode* RootNode, SceneNode* Collideables, SceneNode* Player);
	enum State{ Patrol, Chase};

	void SetWayPoints(SceneNode* P1, SceneNode* P2, SceneNode* P3);
private:
	State m_state;
	SceneNode* m_waypoints[3];
	int	m_waypointCounter = 0;
	float m_speed = 0.08f;
};