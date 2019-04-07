#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Entity.h"
#include "j1Window.h"
#include "CollisionQuadTree.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"


j1Colliders::j1Colliders() : j1Module()
{
	name.assign("colliders");
	
	matrix[COLLIDER_ENTITY][COLLIDER_ENTITY] = true;

}
// Destructor
j1Colliders::~j1Colliders()
{
}

bool j1Colliders::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	debug = node.child("debug").attribute("value").as_bool();
	return ret;
}
bool j1Colliders::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdateCollision", Profiler::Color::Yellow);
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
// Called before render is available
bool j1Colliders::Update(float dt)
{
	BROFILER_CATEGORY("UpdateCollision", Profiler::Color::Red);
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		traditionalMetod = !traditionalMetod;
	}
	if(traditionalMetod)
	{
		Collider* c1;
		Collider* c2;
		for (uint i = 0; i < colliders.size(); ++i)
		{
			// skip empty colliders
			if (colliders[i] == nullptr)
				continue;
			c1 = colliders[i];
			// avoid checking collisions already checked
			for (uint k = i + 1; k < colliders.size(); ++k)
			{
				// skip empty colliders
				if (colliders[k] == nullptr)
					continue;
				c2 = colliders[k];
				++App->scene->collisionsCheck;
				if (c1->CheckCollision(c2->rect) == true)
				{
					OnCollisionCall(c1, c2, dt);
				}
			}
		}
	}
	DebugDraw();

	return true;
}
void j1Colliders::OnCollisionCall(Collider * c1, Collider * c2, float dt)
{
	if (c1->to_delete == false && c2->to_delete == false) {
		if (matrix[c1->type][c2->type] && c1->callback) {
			c1->callback->OnCollision(c1, c2, dt);
		}
		if (c1->to_delete == false && c2->to_delete == false) {
			if (matrix[c2->type][c1->type] && c2 != nullptr && c2->callback != nullptr)
				c2->callback->OnCollision(c2, c1, dt);
		}
	}
}
void j1Colliders::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		LOG("You can see the colliders");
		debug = !debug;
	}

	if(debug) {
		Uint8 alpha = 80;
		for (uint i = 0; i < colliders.size(); ++i)
		{
			if (colliders[i] == nullptr)
				continue;
			switch (colliders[i]->type)
			{
			case COLLIDER_NONE: // white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_ENTITY: // red
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
				break;
			default: //white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			}
		}
	}
}
bool j1Colliders::checkColisionList(Collider * enemCollider)
{
	for (uint i = 0; i < colliders.size(); ++i) {
		if (colliders[i] == nullptr)
			continue;
		if (enemCollider->CheckCollision(colliders[i]->rect))
			return true;
	}
	return false;
}
// Called before quitting
bool j1Colliders::CleanUp()
{
	LOG("Freeing all colliders");
	for (uint i = 0; i < colliders.size(); ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Entity * callback)
{
	Collider* col = new Collider(rect, type, callback);
	colliders.push_back(col);
	return col;
}
bool j1Colliders::EraseAllCollider(Collider* collider)
{
	std::vector<Collider*>::iterator item = colliders.begin();
	for (;item < colliders.end(); ++item)
	{
		if (*item == collider)
		{
			(*item)->to_delete = true;
		}
	}
	return false;
}

// -----------------------------------------------------
bool Collider::CheckCollision(const SDL_Rect& r) const
{
	if (r.x > rect.x + rect.w || r.x + r.w<rect.x || r.y>rect.y + rect.h || r.y + r.h < rect.y)
		return false;
	return true;
}

bool j1Colliders::Check(Collider* c1, COLLIDER_TYPE type) {

	Collider* c2 = nullptr;
	// avoid checking collisions already checked
	for (uint k = 0; k < colliders.size(); ++k)
	{
		// skip empty colliders
		if (colliders[k] == nullptr || colliders[k]->type != type)
			continue;
		c2 = colliders[k];
		if (c1->CheckCollision(c2->rect) == true)
		{
			return true;
		}
	}

	return false;
	
}