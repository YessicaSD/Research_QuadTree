#include "CollisionQuadTree.h"
#include "j1EntityManager.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "j1App.h"

CollisionQuadTree::CollisionQuadTree(uint max_levels, SDL_Rect area, uint level, uint maxElements, CollisionQuadTree * prev):
	Quadtree(max_levels, area, level, maxElements), prev(prev)
{
	CleanUp();
}

CollisionQuadTree::~CollisionQuadTree()
{
	CleanUp();
}

void CollisionQuadTree::Subdivide()
{
	//TODO2:
	//implement Subdivide function.
	//Create 4 childs that are divisions of the father.

}

void CollisionQuadTree::AddCollider(Collider* col)
{

	//TODO1:
	/*Implement AddCollider function();
	This function only adds colliders in the tree’s level  where it’s called.
	If the size of the Elements list  is smaller than maxElements call subdivide function.*/
}

void CollisionQuadTree::DistrbuteColliders()
{
	std::list<Collider*>::iterator item = elements.begin();
	for (;item != elements.end();++item)
	{
		//TODO3:
		/*Create the loop which distributes the colliders between the 4 new  quadtrees
		Remember to delete colliders when saved in a subnode
		Call this function in subdivide */
		
		//TODO4:
		//creates the condicion to call subdivide function.
		
	}
}


void CollisionQuadTree::CheckCollisions(float dt)
{
	//follow the camera

	Collider* c1;
	Collider* c2;

	for (std::list<Collider*>::iterator item = elements.begin(); item != elements.end(); ++item)
	{
		// skip empty colliders
		if (*item == nullptr)
			continue;
		c1 = (*item);
		// avoid checking collisions already checked
		std::list<Collider*>::iterator item2 = ++item;
		--item;
		for (; item2 != elements.end(); ++item2)
		{
			// skip empty colliders
			if (*item2 == nullptr)
				continue;
			c2 = *item2;
			++App->scene->collisionsCheck;
			if (c1->CheckCollision(c2->rect) == true)
			{
				if (c1->to_delete == false && c2->to_delete == false) {
					App->collider->OnCollisionCall(c1, c2, dt);
				}
			}
		}
		if (isDivided)
		{
			for (int i = 0; i < 4; ++i)
			{
				//TODO6:
				//Take the function that checks collisions in the same level and implement it to check the collision with its childs.

				nodes[i]->CheckCollisions(dt);
			}
		}
	}
	Draw();


	
}

void CollisionQuadTree::PlaceCollider(Collider * col)
{

	if (CheckIn(col->rect))
	{
		//TODO5:
	/*implements the method that puts a collider in its place in the quadTree
		uncomment the commented parts.It also implements the function when is divided
		Uncomment function in Scene.cpp*/
		//use by check if collider are inside one of the nodes
		//bool isIn = false;
		//if (isDivided)
		//{
		
			
		//}
		//else
			//AddCollider(col);
	}
}

void CollisionQuadTree::CleanUp()
{
	if (isDivided)
	{
		for (int i = 0; i < 4; ++i)
		{
			
			if (nodes[i] != nullptr)
			{
				nodes[i]->CleanUp();
				RELEASE(nodes[i]);
			}
		}
		isDivided = false;
	}
	if(!isDivided)
	{
		std::list<Collider*>::iterator item = elements.begin();
		for (; item != elements.end(); ++item)
		{
			elements.remove(*item);
		}
		elements.clear();
	}
}

uint CollisionQuadTree::GetSize()
{
	uint numElement = 0;

	std::list<Collider*>::iterator item = elements.begin();
	for (; item != elements.end(); ++item)
	{
		++numElement;
	}

	return numElement;
}

void CollisionQuadTree::Draw()
{
	App->render->DrawLine(area.x, area.y, area.x, area.y + area.h, 0, 0, 255);
	App->render->DrawLine(area.x, area.y, area.x + area.w, area.y, 0, 0, 255);
	App->render->DrawLine(area.x, area.y + area.h, area.x + area.w, area.y + area.h, 0, 0, 255);
	App->render->DrawLine(area.x + area.w, area.y, area.x + area.w, area.y + area.h, 0, 0, 255);
	if (isDivided)
	{
		for (uint i = 0; i < 4; ++i)
		{
			nodes[i]->Draw();
		}
	}	
}
