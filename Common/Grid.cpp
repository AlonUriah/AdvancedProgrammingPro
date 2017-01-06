//============================================================================
// Name        : grid.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Grid has width and height members indicating its size.
//				 By default its size is 10x10.
//				 InitGrid will create nodes according to their axis values by default
//============================================================================

#include "Grid.h"
#include "Node.h"
#include "Obstacle.h"
#include <stdexcept>
#include <typeinfo>

using namespace std;


/*
 * Default constructor creates a 10x10 grid
 * and call initGrid to create the nodes.
 */
Grid::Grid() {
	_width = 10;
	_height = 10;
	initGrid();
}

/*
 * Creates a grid with given sizes
 * and call initGrid to create the nodes.
 */
Grid::Grid(int width, int height){
	if(width <= 0 || height <= 0){
		throw invalid_argument("Invalid values for Grid's width and height.");
	}
	_width = width;
	_height = height;
	initGrid();
}

/*
 * After this grid was created (with given/default sizes)
 * this method is being called at initialize _matrix of nodes.
 */
void Grid::initGrid(){
	/*
	 * Matrix is represented as 1d array of pointers
	 * cell (x,y) will be _matrix[x*width + y].
	 */
	_matrix = new Node*[_height*_width];

	for(int x=0; x<_width; x++){
		for(int y=0; y<_height; y++){
			Point current(x,y);
			_matrix[(y*_width) + x] = new Node(current);
		}
	}
}

/*
 * Returns a pointer to a node at a given location
 * the point (x,y) will be stored at:
 * x*witdh + y
 */
Node* Grid::getNodeAt(Point location){
	int x= location.getX();
	int y = location.getY();
	if(x < 0 || y < 0){
		throw invalid_argument("Bad query, cannot access negative indexes");
	}
	return _matrix[(y*_width) + x];
}

/*
 * Grid is responsible for 'living the map'
 * It knows whats each node's status - available or not
 * and creates a vestor of neighbors accordingly.
 * This is a searchable method that node is overriding
 */
vector<Searchable*> Grid::getNodesNeighbors(Searchable* n1){
	Point current = n1->getValue();
	vector<Searchable*> neighbors;

	/*
	 * At most, each node has 4 neighbors - up,down,left,right.
	 * Check: (1) if the potential neighbor is in scope (no negative indexes)
	 * (2) if neighbor is not an obstacle
	 * If one's neighbor is an obstacle - ignore it, don't count
	 * it as a neighbor.
	 */
	if(current.getX()-1 >= 0){
		Point left(current.getX()-1,current.getY());
		Node* neighbor = getNodeAt(left);
		if(neighbor->isAvailable()){
			neighbors.push_back(getNodeAt(left));
		}
	}

	if(current.getY()+1 < _height){
			Point up(current.getX(),current.getY()+1);
			Node* neighbor = getNodeAt(up);
			if(neighbor->isAvailable()){
				neighbors.push_back(getNodeAt(up));
			}
	}

	if(current.getX()+1 < _width){
		Point right(current.getX()+1,current.getY());
		Node* neighbor = getNodeAt(right);
		if(neighbor->isAvailable()){
			neighbors.push_back(getNodeAt(right));
		}
	}

	if(current.getY()-1 >= 0){
		Point down(current.getX(),current.getY()-1);
		Node* neighbor = getNodeAt(down);
		if(neighbor->isAvailable()){
			neighbors.push_back(getNodeAt(down));
		}
	}

	//The order demand (9,12,3,6) is implemented by the order of push
	return neighbors;
}

/*
 * Write this Grid by printing its sizes
 */
ostream& operator << (ostream& os, const Grid &grid){
	return os << "Grid sizes: " << grid._width << "x" << grid._height << endl;
}

/*
 * Returns grid's height
 */
int Grid::getHeight(){
	return _height;
}

/*
 * Returns grid's width
 */
int Grid::getWidth(){
	return _width;
}

/*
 * Swap node at location 'location' with 'newNode'.
 * Make sure to delete the old node.
 */
void Grid::swapNodes(Point location, Node* newNode){
	Node* oldNode = getNodeAt(location);
	_matrix[(location.getY()*_width)+location.getX()] = newNode;
	delete oldNode;
}

/*
 * Grid allocates room for a node pointers array so
 * it frees that data in its end of scope and also
 * delete each node inside
 */
Grid::~Grid() {
	//Node* currentNode = 0;

	for(int x=0; x<_width; x++){
		for(int y=0; y<_height; y++){
			Point current(x,y);
			//currentNode = getNodeAt(current);
			delete	getNodeAt(current);//currentNode;//getNodeAt(current);
		}
	}
	delete[] _matrix;
}

void Grid::resetNodesDistance(){
	for(int i=0; i<_width; i++){
		for(int j=0; j<_height; j++){
			Point location(i,j);
			Node* curr = getNodeAt(location);
			curr->setDistance(-1);
		}
	}
}

void Grid::resetNodesParents(){
	for(int i=0; i<_width; i++){
		for(int j=0; j<_height; j++){
			Point location(i,j);
			Node* curr = getNodeAt(location);
			Searchable* dummy = 0;
			curr->setParent(dummy);
		}
	}
}
