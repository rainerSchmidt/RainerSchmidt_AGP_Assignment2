#pragma once
#include "scenenode.h";

class Collectable
{
public:
	Collectable();
	~Collectable();
	void Logic(SceneNode* Node, SceneNode* RootNode, SceneNode* Player);

private:
	//float m_frameCount;
};
