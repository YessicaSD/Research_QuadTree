#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include "Animation.h"
#include <string>
#include "j1Collision.h"
#include "SDL_image/include/SDL_image.h"


struct SDL_Texture;
struct Collider;

struct TileSetEntity {

	SDL_Rect GetTileRect(int id) const;

	std::string name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	std::string imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

enum class EntityState {
	IDLE = 0,
	WALKING,

	UNKNOWN
};


class j1Entity
{

public:

	j1Entity(int x, int y, std::string name);
	virtual ~j1Entity();

	const Collider* GetCollider() const;

	//-----------------------------------------------------------------------------------------------------------
	virtual bool Start() { return true; }
	virtual bool Update(float dt);
	virtual void Draw();
	void OnCollision(Collider* collider1, Collider * collider2, float dt);
	virtual bool CleanUp();
	
public:
	uint count = 0;

	std::string name;

	fPoint position;

	iPoint pivot = { 0, 0 };

	iPoint size = { 0, 0 };

	Animation anim_run;
	Animation col_anim;
	SDL_Texture* texture;

	Collider* collider;

	std::string folder = "entities/";

	SDL_RendererFlip flip = (SDL_RendererFlip)SDL_FLIP_NONE;

	pugi::xml_document	entity_file;
	
	Animation* current_animation = nullptr;

protected:
	bool goleft;
	

};
#endif // __ENTITY_H__