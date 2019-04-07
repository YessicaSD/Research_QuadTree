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
	//Implement Subdivide function
	//be careful with the maxim_levels
	//TODO3
	//use DistrivuteColliders

}

void CollisionQuadTree::AddCollider(Collider* col)
{

	//TODO1:
	//Create funtion to add collider in a quadTree
	//Remember! if level pass the maxElements subdivide
}

void CollisionQuadTree::DistrbuteColliders()
{
	std::list<Collider*>::iterator item = elements.begin();
	for (;item != elements.end();++item)
	{
		//TODO3:
		//create the loop by distribute the colliders between the new 4 quadtree
		//remember delete collider when save in subnode		
		
		//TODO4:
		//Comprove that if one node pass the maxElements it's subdivided again.
		
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
				//Take de function for check collisions in the same level and implement for check the collision with his sons
			

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
		//use by check if collider are inside one of the nodes
		//bool isIn = false;
		//if (isDivided)
		//{
			//TODO5:
			//implement method by put one collider in his place in the quadTree
			
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
