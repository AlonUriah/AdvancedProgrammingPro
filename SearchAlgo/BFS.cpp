//============================================================================
// Name        : BFS.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Contains BFS algorithm in a static method
//				 that gets a grid, a start and an end point and return the
//				 shortest root.
//				 Generic will be achieved by changing map to ANY different
// 				 data structure that will implement getNeighbors method that return
//				 searchables.
//============================================================================

#include "BFS.h"
#include <queue>
#include <stack>

/*
 * Default empty constructor.
 * There is no use of this one.
 * BFS gives static methods access.
 */
BFS::BFS() { }

/*
 * Returns shortest root in a stack.
 * Destination is on top.
 * Based on wikipedia algorithm for BFS
 */
stack<Searchable*> BFS::getRoot(Grid& map, Searchable* start, Searchable* end){

	map.resetNodesDistance();
	map.resetNodesParents();

	queue<Searchable*> visited;
	vector<Searchable*> neighbors;
	stack<Searchable*> root;
	start->setDistance(0);

	visited.push(start);
	while(!visited.empty()){
		Searchable* current = visited.front();
		visited.pop();

		//Return the root as soon as you get to destination
		if(*current == *end){
			Searchable* currCopy = current;
			root.push(currCopy);
			while(current->getParent()){
				Searchable* parentCopy = current->getParent();
				root.push(parentCopy);
				current = (current->getParent());
			}
			return root;
		}

		neighbors.clear();
		//getNeighbors returns a vector build on 9,12,3,6 order
		neighbors = map.getNodesNeighbors(current);
		for(vector<Searchable*>::iterator iter = neighbors.begin();
				iter != neighbors.end(); ++iter){
			if(*iter && (*iter)->getDistance()<0){
				(*iter)->setDistance(current->getDistance()+1);
				visited.push(*iter);
				(*iter)->setParent(current);
			}
		}
			neighbors.clear();
	}
	return root;
}

/*
 * Default destructor.
 * There will be no use of this one.
 * One should not create BFS instances.
 */
BFS::~BFS() { }

