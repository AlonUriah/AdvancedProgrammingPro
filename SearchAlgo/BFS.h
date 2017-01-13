//============================================================================
// Name        : BFS.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Contains BFS algorithm in a static method
//				 that gets a grid, a start and an end point and return the
//				 shortest root.
//				 Generic will be achieved by changing map to ANY different
// 				 data structure that will implement getNeighbors method that return
//				 searchables.
//============================================================================
#include <stack>
#include "Grid.h"
#include "Searchable.h"

#ifndef HEADERS_BFS_H_
#define HEADERS_BFS_H_
using namespace std;

class BFS {
public:
	/*
	 * Default empty constructor.
	 * There is no use of this one.
	 * BFS gives static methods access.
	 */
	BFS();

	/*
	 * Returns shortest root in a stack.
	 * Destination is on top.
	 * Based on wikipedia algorithm for BFS
	 */
	static stack<Searchable*> getRoot(Grid& map, Searchable* start, Searchable* end);

	/*
	 * Default destructor.
	 * There will be no use of this one.
	 * One should not create BFS instances.
	 */
	~BFS();
};

#endif /* HEADERS_BFS_H_ */
