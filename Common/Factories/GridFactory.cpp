//============================================================================
// Name        : GridFactory.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Grid has width and height members indicating its size.
//				 By default its size is 10x10.
//				 GridFactory is created by common.parseGrid and it helps
//				 to build Grids of certain size and maybe more features in the future.
//============================================================================
#include "GridFactory.h"
#include "Grid.h"

/*
 * By default factory creates Grids
 * with x=10 and y=10.
 */
GridFactory::GridFactory(){
	_x = 10;
	_y = 10;
}

/*
 * Sets grid factory x and y attributes
 * to create Grids of these sizes
 */
GridFactory::GridFactory(int x, int y){
	_x = x;
	_y = y;
}

/*
 * Creates Grids with the value of x and y
 */
Grid* GridFactory::create(){
	return new Grid(_x,_y);
}
