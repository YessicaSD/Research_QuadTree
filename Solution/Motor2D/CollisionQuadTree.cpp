#include "CollisionQuadTree.h"
#include "j1EntityManager.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Window.h"
#include "Brofiler\Brofiler.h"
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
	//if (level < max_levels)
	{
		isDivided = true;
		//topLeft
		nodes[0] = new CollisionQuadTree(max_levels, { area.x,area.y, area.w / 2, area.h / 2 }, level + 1, maxElements, this);
		//topRight
		nodes[1] = new CollisionQuadTree(max_levels, { area.x + area.w / 2,area.y,area.w / 2, area.h / 2 }, level + 1, maxElements, this);
		//downLeft
		nodes[2] = new CollisionQuadTree(max_levels, { area.x,area.y + area.h / 2 , area.w / 2, area.h / 2 }, level + 1, maxElements, this);
		//downRight
		nodes[3] = new CollisionQuadTree(max_levels, { area.x + area.w / 2 ,area.y + area.h / 2, area.w / 2,  area.h / 2 }, level + 1, maxElements, this);
		DistrbuteColliders();
	}
}

void CollisionQuadTree::AddCollider(Collider* col)
{

	//TODO1:
	//Create funtion to add collider in a quadTree
	//Remember! if level pass the maxElements subdivide
	elements.push_back(col);
	if (elements.size() >= maxElements)
	{
		Subdivide();
	}

}

void CollisionQuadTree::DistrbuteColliders()
{
	
	std::list<Collider*>::iterator item = elements.begin();
	for (;item != elements.end();++item)
	{
		//TODO3:
		/*create the loop by distribute the colliders between the new 4 quadtree
		remember delete collider when save in subnode
		call the function in subdivide*/

		bool quit = false;
		for (uint i = 0; i < 4; ++i)
		{
			if (nodes[i]->CheckIn((*item)->rect))
			{
				nodes[i]->elements.push_back(*item);
		//TODO4:
		//Comprove that if one node pass the maxElements it's subdivided again.
				if (nodes[i]->GetSize() >= maxElements)
				{
					nodes[i]->Subdivide();
				}
				quit = true;
			}
		}
		if (quit)
			elements.remove(*item);
	}
}


void CollisionQuadTree::CheckCollisions(float dt)
{
	//follow the camera
	BROFILER_CATEGORY("QuadTreeCollisions", Profiler::Color::Red);

	Collider* c1;
	Collider* c2;

	if(isDivided)
	{
		for (int i = 0; i < 4; ++i)
		{
		//TODO6:
		//Take de function for check collisions in the same level and implement for check te collision with his sons
			if (elements.size() != 0)
			{
				std::list<Collider*>::iterator item = elements.begin();
				for (; item != elements.end(); ++item)
				{
					// skip empty colliders
					if (*item == nullptr)
						continue;
					c1 = (*item);
					// avoid checking collisions already checked

					std::list<Collider*>::iterator item2 = nodes[i]->elements.begin();
					for (; item2 != nodes[i]->elements.end(); ++item2)
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
				}
			}
			nodes[i]->CheckCollisions(dt);
		}
	}
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
	}
	Draw();


	
}

void CollisionQuadTree::PlaceCollider(Collider * col)
{
	BROFILER_CATEGORY("PutEntitiesinQuadtree", Profiler::Color::Red);
	//TODO5:
		//implement method by put one collider in his place in the quadTree
		//discoment part commented, you implement the condicional if is divided
		//be caregul! 2nd part is in scene

	if (CheckIn(col->rect))
	{
		bool isIn = false;
		if (isDivided)
		{
			
			for (int i = 0; i < 4; ++i)
			{
				if (nodes[i]->CheckIn(col->rect))
					isIn = true;
			}
			if (isIn)
			{
				for (int i = 0; i < 4; ++i)
				{
					nodes[i]->PlaceCollider(col);
				}
			}
			else
				AddCollider(col);
		}
		else
			AddCollider(col);
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
