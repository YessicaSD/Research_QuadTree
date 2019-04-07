#include "j1App.h"
#include "ent_NPC.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "Brofiler/Brofiler.h"

ent_NPC::ent_NPC(int x, int y, std::string name) : j1Entity(x, y, name) 
{
	
}

ent_NPC::~ent_NPC()
{
}

bool ent_NPC::Start()
{
	texture = App->tex->Load("entities/hero.png");

	anim_run.PushBack({ 17,33,27,30 });
	anim_run.PushBack({ 81,36,27,28 });
	anim_run.speed = 4.5;
	anim_run.loop = true;

	col_anim.PushBack({16,82,28,31});
	col_anim.PushBack({83,83,23,30});
	col_anim.PushBack({149,82,21,31});
	col_anim.PushBack({213,81,28,32});
	col_anim.speed = 6;
	col_anim.loop = false;


	pivot.create(27 / 2, 25);
	size.create(27, 30);
	current_animation = &anim_run;

	collider = App->collider->AddCollider({(int)position.x, (int)position.y,size.x,size.y}, COLLIDER_TYPE::COLLIDER_ENTITY, this);
	return true;
}

bool ent_NPC::Update(float dt)
{
	BROFILER_CATEGORY("UpdatePlayer", Profiler::Color::Red);

	current_animation->GetCurrentFrame(dt);

	App->scene->quadtree->PlaceCollider(collider);
	
	iPoint enemy_pos = App->map->WorldToMap(position.x, position.y);
	bool exist = false;
	iPoint objective;
	if (goleft)
		objective = iPoint(enemy_pos.x - 1, enemy_pos.y - 1);
	else
		objective = iPoint(enemy_pos.x + 1, enemy_pos.y + 1);
	
	if (App->map->data.width - 1 < objective.x || App->map->data.height - 1 < objective.y || objective.x == -1 || objective.y == -1)
		goleft = !goleft;
	
	if (App->map->data.width - 1 >= objective.x && App->map->data.height - 1 >= objective.y && objective.x >= -1 && objective.y >= -1)
	{
		if (goleft)
		{
			position.x -= 20 * dt;
			position.y -= 20 * dt;
			flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
		}
		else
		{
			position.x += 20 * dt;
			position.y += 20 * dt;
			flip = SDL_RendererFlip::SDL_FLIP_NONE;
		}
	}
	if (current_animation->Finished())
		current_animation = &anim_run;

	return true;
}

// Called before quitting
bool ent_NPC::CleanUp()
{
	return App->tex->UnLoad(texture);
	if (collider != nullptr)
		collider->to_delete = true;
	current_animation = nullptr;

}

void ent_NPC::OnCollision(Collider* c1, Collider* c2, float dt) 
{}

