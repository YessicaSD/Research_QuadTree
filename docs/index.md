# QuadTree

In videogoames, when we want to draw a map on camera or verify if two entities/ particles are colliding, we use the method we have learned previously. This consists on an exhaustive method that consist of going element by element to perform the desired action. 
Now lets analyze this method. If we have 10 collision we shall check each particle colliding with the other 9. So we finally make 10 * 10 = 100 checks. In case that we have 100 collisions we will make 100 * 100 = 10.000 checks and if we have 1000 collision -> 1000 * 1000 = 1.000.000 checks. We can see that the check collisions have a cost of elements^2. 

## Tree

When we talk about programming, a tree is one of the most usefull data structure. Its principal characteristic is that it stores data in hierarchical structure. While arrays, list, etc... do it in a lineal form. Trees are compounded by nodes(which store the information) and branches (the links between the nodes).

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/tree.jpg?raw=true" width="520">
</p>

## QuadTree

A QuadTree is a tree in which each node has either 4 or 0 childs. We use this data structure for 2 reasons:

* **Camera Culling:** Instead of showing the tiles from the map that belong to the camera by doing an exhaustive search (which is highly uneficient) we use the QuadTree structure. This lets us achieve a cost of n * log(base 4) n (being n the size of the map). We make this happen by recursively checking one fourth of the map with our camera boundaries. If there is any overlaps we divide this quarter in 4 pieces again. We continue doing this until we reach the maximum number of divisions set by the programmer. By doing this we end up having the tiles that must be shown on camera and a few extra ones to have a margin and not have any tile being cut.
As a fact, the QuadTree that we use is a static type of data structure. This is because we save the map tiles inside of it once we load the map. Then we only have to reffer to the QuadTree to acces the desired tile/s.

* **Check Collisions:** This case is more difficult than camera culling because the set of tiles is static but entities and particles are dynamic. In this case we need to create a dynamic QuadTree that always changes along with entities. This it's slower than the quadtree mentioned before but it's faster than the exhaustive method. 

In this research I focused in check collisions because learning to develop this quadtree is more complex.

## Collsions - QuadTree

The principal idea of this method is that entities search its position inside the tree and only compare with others that are in the same position.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2000.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2001.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2002.png?raw=true" width="300">
   <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2003.png?raw=true" width="300">
</p>

This process avoids checking entities collisions that are imposible due to the distance in between them.
With 15 entities in the map we can see this:

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown%202.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown3.png?raw=true" width="300">
</p>

On the left side, we can see a profiler cheking collisions using a quadtree. While on the right we see it using the exhaustive method. In this case it is shown that the times are quite similar. But when we have 300/400 entities on screen this happens:

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown%204.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown.png?raw=true" width="300">
</p>
Quadtree its faster than the exhaustive method. 

## Code

Now I will explain the code.

First remember that this code it's focused on a collisions quadtree.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/quadtreeH.PNG?raw=true" width="650">
</p>

This is my "quadtree" class. We use it to store the variables and implement the functions that will be used in all types of quadtrees. So all the others will depen on this one.

The constructor needs the maximum number of levels, the area to envolve a node, the level into the quadtree and the maximum number of elements that a quadtree can store before subdividing.

Functions: 

* Subdivide: Creates 4 child nodes when it is passed the total number of elements.
* Draw: Paint the quadtree.
* CheckIn: Check if one rectangle is inside the area.

Now we move to collisions quadtree.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/CollisionquadtreeH.PNG?raw=true" width="650">
</p>

Since this class is a child of a quadtree, the constructor needs the same variables as a quadtree and one pointer the quadtree that create itself. If this is the first quadtree the pointer equals nullptr;

Functions:
* Subdivide: inherited from quadtree
* AddCollider: this function takes care of saving the colliders where other functions tell it. If this node exceeds the maximum number of elements AddColliders calls to subdivide.
* DistribureColliders: this functions distributes the colliders between the childs when dividing.
* PlaceCollider: This function distributes the colliders from the entities to its right place in the tree.
* CleanUp: clean tree.
* GetSize: returns the number of elements that a node contains.
* Draw: inherited from quadtree.

variables:
* nodes[i]: vector cointaining the quadtree's childs.
* elements: elements into a quadtree.
* prev: pointer to its father.

## TODOS

### TODO 1

download todos [here](https://github.com/didaclis/Research_QuadTree).

* **statement:**

Implement funciton AddCollider. This function only adds the collider into the list of elements and if the size of elements is bigger than the maximum number of allowed elements, it calls the subdivide function.

* **comprove:**

number of entities incrase.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/todo1.PNG?raw=true" width="450">
</p>

* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO1Sol.PNG?raw=true" width="450">
</p>

### TODO 2

* **statement:**

Implement funciton Subdivide. Assign 4 quadtrees to nodes vector.

* **comprove:**

When pass the maxim elements appear 4 rectangles inside the original.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO2.PNG?raw=true" width="450">
</p>
* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO2Sol.PNG?raw=true" width="450">
</p>
### TODO 3

* **statement:**

It implements function to distribute the colliders between its childs. Calls the funtion in subdivde method. 

* **comprove:**

--

* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO3Sol.PNG?raw=true" width="450">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO3SOL2.PNG?raw=true" width="450">
</p>

### TODO 4

* **statement:**

If while distributing elements one node size is bigger than maxElements call the subdivide function.

* **comprove:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/todo4.PNG?raw=true" width="450">
</p>

* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO4Sol.PNG?raw=true" width="450">
</p>


### TODO 5

* **statement:**

Implement funciton PlaceCollider. Add the collider at the biggest level that acomplishes CheckIn conditions. Afterwards, go to scene.cpp and uncomment the function.
* **comprove:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5.1.PNG?raw=true" width="450">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5.2.PNG?raw=true" width="450">
</p>
* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5SOL.PNG?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5SOL2.PNG?raw=true" width="300">
</p>
Before do "TODO 6" I shall explain the problem that it solves. With the function CheckCollision implemented at this moment the quadtree checks the collisions with colliders in its node, but what happens when the collider isn't in one node in the maximum level of this quadtree? At the last "TODO" we saved in the last node to complit the CheckIn boolean. This is the result: 

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/QuadTreeCodeTree.gif?raw=true" width="300">
  </p>
 To detect collisions with this nodes we need to check the collider with elements that are in the child nodes.

### TODO 6

* **statement:**

Take the code of detection collisions and implement it so it checks the functions with its sons.

* **comprove:**

Detect collision on lines. 

* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO6SOL.PNG?raw=true" width="450">
</p>
## Autor

* Name: Dídac Llop i Serrna.
* Github [didaclis](https://github.com/didaclis).
* mail: dllop.9@gmail.com

# Links to Resources
[genbeta.com](https://www.genbeta.com/desarrollo/teoria-de-colisiones-2d-quadtree). 

[the coding train](https://www.youtube.com/watch?v=OJxEcs0w_kE).

[jimkang](https://jimkang.com/quadtreevis/).

[GeeksforGeeks](https://www.geeksforgeeks.org/quad-tree/).

[Wikipedia](https://es.wikipedia.org/wiki/Quadtree).

I have taken the base code from [Sprite Ordering and Camera Culling Research](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research) by [christt105](https://github.com/christt105) and handouts worked in development.







