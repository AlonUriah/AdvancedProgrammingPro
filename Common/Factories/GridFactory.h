//============================================================================
// Name        : GridFactory.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Grid has width and height members indicating its size.
//				 By default its size is 10x10.
//				 GridFactory is created by common.parseGrid and it helps
//				 to build Grids of certain size and maybe more features in the future.
//============================================================================
#include <vector>
#include "Grid.h"

#ifndef HEADERS_GRID_FACTORY_
#define HEADERS_GRID_FACTORY_

//dummy
class GridFactory{
private:
	int _x;
	int _y;
public:
	/*
	 * By default factory creates Grids
	 * with x=10 and y=10.
	 */
	GridFactory();

	/*
	 * Sets grid factory x and y attributes
	 * to create Grids of these sizes
	 */
	GridFactory(int x, int y);

	/*
	 * Creates Grids with the value of x and y
	 */
	Grid* create();
};

#endif
