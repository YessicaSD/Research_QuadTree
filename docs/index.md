# QuadTree

In videdgoames, when we want to draw map in camera or verify if two entities/ particles are colliding, we believe use that the method we know. We use an exhaustive method that consist of going element by element to perform the desired action. Know we will analyze this method. If we have 10 collision we check each collison with other 9, so we finally make 10 * 10 = 100 checks. In case that we have 100 collisions we will make 100 * 100 = 10.000 checks and if we have 1000 collision -> 1000 * 1000 = 1.000.000 checks in frame. We can see that the check collisions have a cost to elements^2. 

## Tree

When we talk to programming, one tree is one of the more usefull structure of data. His principal caracteristic is that store data in hierarchical structure when the arrays, list, etc... do it in a lineal form. Trees are compost by nodes(store the information) and branch (link nodes).

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/tree.jpg?raw=true" width="520">
</p>

## QuadTree

QuadTree is one tree where Nodes have 4 or 0 childs. We use this structure for 2 reasons:

* **Camera Culling:** As I mentioned, we use the exhaustive method to only paint the tiles that are represented in camera. But for the big map pass same that colliders. By solve this we use one quadtree. In this case only we need to agroup tiles in nodes of quadtree, and then check to recursive form if the rect that compose the tree is on camera. If it is, we need to check what to his sons stay into camera. When we arrive under the tree we paint de tiles of the nodes that keep on camera.

* **Check Collisions:** This case is more difficult by camera culling because the tiles are static but entities and particles are dinamic. For this case we need to create a dinamic quadtree that always change together with entities. This it's more slow that quadtree mentioned before but it's more fast than exhaustive method. 

In this research I centred in check collisions because if learn this quadtree, the other is most easy to do it. 

## Collsions - QuadTree

The principal idea of this method is that entities search is position inside the tree and how it they know in that node stay, compare the collision with entities in a same node.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2000.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2001.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2002.png?raw=true" width="300">
   <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/1366_2003.png?raw=true" width="300">
</p>

This process avoid check a lot of collisions with entities that it's imposible to collide by the distance.
With a 15 entities in map we can whatch this:

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown%202.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown3.png?raw=true" width="300">
</p>

In the left we can see a profiler when cheking collisions with quadtree and the right with exhaustive method. We can see that the values are very similar. But when we have 300/400 entities in screen succes this:

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown%204.png?raw=true" width="300">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/unknown.png?raw=true" width="300">
</p>
Quadtree its more faster than exhaustive method. 

## Code

Now I pass to explain the code.

First remember that this code it's enfoqued to collisions quadtree.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/quadtreeH.PNG?raw=true" width="650">
</p>

This is my class quadtree when I store the variables and implement the functions that will be use all types of quadtrees.

The constructor need the maxim number of levels, the area to envolve this node, the level into quadtree and the maxim numbre of elements that quadtree store before subdivide.

functions: 

* Subdivide: when the node pass the maxim numbres of elements create 4 child nodes.
* Draw: Paint the quadtree.
* CheckIn: Comprove if one rect are iside the area.

Now we pass to collisions quadtree.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/CollisionquadtreeH.PNG?raw=true" width="650">
</p>

How this class are a child of quadtree, the constructor need the same variables of quadtree and one pointer of quadtree that create this, if is the first quadtree the pointer = nullptr;

functions:
* Subdivide: inherited to quadtree
* AddCollider: this function only charge to save the colliders where other functiton tell his. If this node pass maxim of elements AddColliders call tu subdivide.
* DistribureColliders: this functions is encharge that when tree subdivides, distribute the colliders between the childs.
* PlaceCollider: This function organitzate tree all frames with position to entities colliders.
* CleanUp: clean tree.
* GetSize: return numbrer of elemns that node contains.
* Draw: inherited to quadtree.

variables:
* nodes[i]: vector of childs to quadtree.
* elements: elements into quadtree.
* prev: pointer to father.

## TODOS

### TODO 1

* **statement:**

Implement funciton AddCollider. This function only add the collider in list to elements and if the size of elements is more bigger than the maxim elements call the subdivide function.

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

When pass the maxim elements appear 4 rects inside the original.

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO2.PNG?raw=true" width="450">
</p>
* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO2Sol.PNG?raw=true" width="450">
</p>
### TODO 3

* **statement:**

Implement funciton to distribute the colliders between his childs. Call the funtion in subdivde method. 

* **comprove:**

--

* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO3Sol.PNG?raw=true" width="450">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO3SOL2.PNG?raw=true" width="450">
</p>

### TODO 4

* **statement:**

If when distribute elements one node size it's bigger than maxElements call subdivide function.

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

Implement funciton PlaceCollider. Add the collider in biggest level that comply CheckIn condicion. Then go scene and discomment the function commented

* **comprove:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5.1.PNG?raw=true" width="450">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5.2.PNG?raw=true" width="450">
</p>
* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5SOL.PNG?raw=true" width="450">
  <img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO5SOL2.PNG?raw=true" width="450">
</p>
Before do TODO 6 i need to explain the problem that his solved. With the function CheckCollision implemented in this moment the quadtree check the collisions with colliders in his node, but what happens when the collider isn't in one node in the maxim level of this quadtree? In last todo we saved in the last node to complit the CheckIn boolean. This is the result: 

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/QuadTreeCodeTree.gif?raw=true" width="450">
  </p>
  For dectect collisions with this nodes we need check the collider with elements to save in child nodes.

### TODO 6

* **statement:**

Take the code of detection collisions and implement that check the functions with is sons.

* **comprove:**

Detect collision on lines. 

* **solution:**

<p align="center">
<img src="https://github.com/didaclis/Research_QuadTree/blob/master/docs/images/TODO6SOL.PNG?raw=tru" width="450">
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

i take code base to [Sprite Ordering and Camera Culling Research](https://github.com/christt105/Sprite_Ordering_and_Camera_Culling_Personal_Research) by [christt105](https://github.com/christt105) and handouts worked in development.







