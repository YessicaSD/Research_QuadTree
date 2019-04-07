#ifndef __J1QUADTREE_H__
#define __J1QUADTREE_H__

#include "j1Module.h"


class j1QuadTree
{
public:
	j1QuadTree(SDL_Rect area, uint maxCol, uint lvl);
	~j1QuadTree();

	void DebugDraw();

private:
	SDL_Rect area;
	uint lvl;
	uint maxCol;
};


#endif
