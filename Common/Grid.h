//============================================================================
// Name        : grid.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Grid has width and height members indicating its size.
//				 By default its size is 10x10.
//				 InitGrid will create nodes according to their axis values by default
//============================================================================

#ifndef HEADERS_GRID_H_
#define HEADERS_GRID_H_
#include "Node.h"
#include "Point.h"
#include "Searchable.h"

class Grid {
private:
	int _width;
	int _height;
	Node** _matrix;

	/*
	 * After this grid was created (with given/default sizes)
	 * this method is being called at initialize _matrix of nodes.
	 */
	void initGrid();

public:
	/*
	 * Default constructor creates a 10x10 grid
	 * and call initGrid to create the nodes.
	 */
	Grid();

	/*
	 * Creates a grid with given sizes
	 * and call initGrid to create the nodes.
	 */
	Grid(int width, int height);

	/*
	 * Returns a pointer to a node at a given location
	 * the point (x,y) will be stored at:
	 * x*witdh + y
	 */
	Node* getNodeAt(Point location);

	/*
	 * Grid is responsible for 'living the map'
	 * It knows whats each node's status - available or not
	 * and creates a vestor of neighbors accordingly.
	 * This is a searchable method that node is overriding
	 */
	vector<Searchable*> getNodesNeighbors(Searchable* n1);

	/*
	 * Write this Grid by printing its sizes
	 */
	friend ostream& operator << (ostream& os, const Grid &grid);

	/*
	 * Returns grid's height
	 */
	int getHeight();

	/*
	 * Returns grid's width
	 */
	int getWidth();

	/*
	 * Swap node at location 'location' with 'newNode'.
	 * Make sure to delete the old node.
	 */
	void swapNodes(Point location, Node* newNode);

	/*
	 * Grid allocates room for a node pointers array so
	 * it frees that data in its end of scope and also
	 * delete each node inside
	 */
	~Grid();

	void resetNodesDistance();
	void resetNodesParents();
};

#endif /* HEADERS_GRID_H_ */
