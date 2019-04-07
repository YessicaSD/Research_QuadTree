#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Entity.h"
#include "ent_NPC.h"
#include "j1Window.h"
#include <algorithm>

#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.assign("entities");
}

// Destructor
j1EntityManager::~j1EntityManager()
{
}


bool j1EntityManager::Awake(pugi::xml_node& conf)
{
	LOG("Loading TSX files");
	bool ret = true;
	
	return ret;
}


bool j1EntityManager::Start()
{
	LOG("loading enemies");

	return true;
}

// Called before render is available
bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("UpdateEntityManager", Profiler::Color::Red);

	return UpdateAll(dt);
}

bool j1EntityManager::UpdateAll(float dt)
{
	bool ret = true;
	uint numEntity = 0;
	std::vector<j1Entity*> draw_entities;

	for (std::vector<j1Entity*>::iterator item = entities.begin();item != entities.end(); ++item) {
		++numEntity;
		if (*item != nullptr) {
			ret = (*item)->Update(dt);
			if (App->render->IsOnCamera((*item)->position.x, (*item)->position.y, (*item)->size.x, (*item)->size.y)) {
				draw_entities.push_back(*item);
			}
		}
	}

	std::sort(draw_entities.begin(), draw_entities.end(), j1EntityManager::SortByYPos);

	for (std::vector<j1Entity*>::iterator item = draw_entities.begin(); item != draw_entities.end(); ++item) {
		(*item)->Draw();

	}

	draw_entities.clear();

	static char title[30];
	sprintf_s(title, 30, " | Entities: %u", numEntity);

	App->win->AddStringToTitle(title);

	numEntity = 0;
	return ret;
}



bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdateEntityManager", Profiler::Color::Green);

	return true;
}

// Called before quitting
bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	for (std::vector<j1Entity*>::iterator item = entities.begin(); item != entities.end(); ++item)
	{
		if ((*item) != nullptr) {
			(*item)->CleanUp();
			RELEASE(*item);
		}
	}
	entities.clear();

	return true;
}

j1Entity* j1EntityManager::CreateEntity(int PositionX, int PositionY, std::string name)
{
	j1Entity* ret = new ent_NPC(PositionX, PositionY, name); 
	if (ret != nullptr) {
		entities.push_back(ret);
		ret->Start();
	}
	
	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity * entity)
{
	
	if (entity != nullptr) 
	{
		entity->CleanUp();
		for (std::vector<j1Entity*>::iterator i = entities.begin(); i != entities.end(); ++i) {
			if (*i == entity) {
				delete *i;
				*i = nullptr;
			}
		}
	}
}

bool j1EntityManager::SortByYPos(const j1Entity * ent1, const j1Entity * ent2)
{
	return ent1->pivot.y + ent1->position.y < ent2->pivot.y + ent2->position.y;
}
