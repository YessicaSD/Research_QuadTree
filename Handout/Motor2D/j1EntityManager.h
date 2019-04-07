#ifndef __J1ENTITIES_H__
#define __J1ENTITIES_H__

#include "j1Module.h"
#include "j1Entity.h"
#include <vector>


class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();
	
	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool UpdateAll(float dt);
	bool PostUpdate();
	bool CleanUp();

	j1Entity* CreateEntity(int PositionX, int PositionY, std::string name);
	void DestroyEntity(j1Entity* entity);

	static bool SortByYPos(const j1Entity * ent1, const j1Entity * ent2);

public:

	std::vector<j1Entity*> entities;

};

//struct Sort{
//	bool SortByYPos(const j1Entity * ent1, const j1Entity * ent2);
//}

#endif // __J1ENEMIES_H__
