#include "j1App.h"
#include "j1Entity.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"

#include "j1Window.h"
#include "j1EntityManager.h"

#include "Brofiler/Brofiler.h"


j1Entity::j1Entity(int PositionX, int PositionY, std::string name) : position(PositionX, PositionY), name(name)
{}

j1Entity::~j1Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* j1Entity::GetCollider() const
{
	return collider;
}

bool j1Entity::Update(float dt) {
	BROFILER_CATEGORY("UpdateEntity", Profiler::Color::Red);
	
	return true;
}

void j1Entity::Draw() {

	collider->SetPos(position.x, position.y);

	if (current_animation != nullptr)
		App->render->Blit(texture, position.x, position.y, &current_animation->frames[current_animation->GetNumCurrentFrame()], 1.0F, true, flip);
}

void j1Entity::OnCollision(Collider * collider1, Collider * collider2, float dt)
{
	if(col_anim.Finished())
		col_anim.reset();
	current_animation = &col_anim;
	if (collider1->type == COLLIDER_SCENE || collider2->type == COLLIDER_SCENE)
		goleft = !goleft;
}


bool j1Entity::CleanUp()
{
	bool ret = false;

	ret = App->tex->UnLoad(texture);
	if (collider != nullptr)
		collider->to_delete = true;
	current_animation = nullptr;
	

	return ret;
}


//Functions to help loading data in xml-------------------------------------
//Get the rect info of an id of tileset
SDL_Rect TileSetEntity::GetTileRect(int id) const {
	SDL_Rect rect;
	rect.w = tilewidth;
	rect.h = tileheight;
	rect.x = margin + ((rect.w + spacing) * (id % columns));
	rect.y = margin + ((rect.h + spacing) * (id / columns));
	return rect;
}
