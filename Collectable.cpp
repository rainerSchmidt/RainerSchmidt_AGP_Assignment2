#include "collectable.h"

//Constructor
Collectable::Collectable()
{
	
}

//Destructor
Collectable::~Collectable()
{

}

//Logic for the Collectable objects
//was supposed to include more and random movement
void Collectable::Logic(SceneNode* Node, SceneNode* RootNode, SceneNode* Player)
{
	Node->Rotate(3.0f, RootNode, Player);
}