#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.assign("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& conf)
{
	LOG("Loading Scene");

	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	bool ret = false;

	ret = App->map->Load("iso.tmx"); //Load first map in config.xml

	App->render->camera = App->render->CameraInitPos();

	SDL_Rect map; 
	map.x = -App->map->data.width * App->map->data.tile_width / 2;
	map.y = 0;
	map.w = App->map->data.width * App->map->data.tile_width;
	map.h = App->map->data.height * App->map->data.tile_height;

	quadtree = new CollisionQuadTree(3, map, 0, 6);

	return ret;
}


// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("UpdateScene", Profiler::Color::Red);

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 300 * dt;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 300 * dt;


	//put entity
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint coords = App->render->ScreenToWorld(x, y);	
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
		App->entities->CreateEntity(coords.x, coords.y, "hero");


	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		App->quit_game = true;

	
	App->map->Draw();
	std::list<Collider*>::iterator iter = App->map->data.mapColliders.begin();
	for (; iter != App->map->data.mapColliders.end(); ++iter)
	{
		quadtree->PlaceCollider(*iter);
	}

	if(!App->collider->traditionalMetod)
		quadtree->CheckCollisions(dt);

	static char title[30];
	sprintf_s(title, 30, " | Collisions Checked: %u", collisionsCheck);

	App->win->AddStringToTitle(title);

	collisionsCheck = 0;
	//TODO5
	//Discomment the next function
	quadtree->CleanUp();
	
	return true;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	return App->entities->CleanUp();
}