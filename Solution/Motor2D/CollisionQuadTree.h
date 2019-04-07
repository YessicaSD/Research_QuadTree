#ifndef _COLLISIONQUADTREE_H_
#define _COLLISIONQUADTREE_H_

#include "Quadtree.h"
#include "j1Collision.h"
#include <list>
class CollisionQuadTree : public Quadtree
{
public:
	CollisionQuadTree(uint max_levels, SDL_Rect area, uint level, uint maxElements, CollisionQuadTree* prev = nullptr);
	~CollisionQuadTree();

	void Subdivide();

	void AddCollider(Collider* col);

	void DistrbuteColliders();

	void CheckCollisions(float dt);

	void PlaceCollider(Collider* col);

	void CleanUp();

	uint GetSize();

	void Draw();

private:

	CollisionQuadTree*			nodes[4]; //next subdivide

	std::list<Collider*>		elements;//elements to check collisions

	CollisionQuadTree*			prev = nullptr; //save the Quadtree that create them


};

#endif
