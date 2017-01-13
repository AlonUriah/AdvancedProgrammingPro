/*
 * Logic.h
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */

#ifndef LOGIC_H_
#define LOGIC_H_

#include "Grid.h"
#include "TaxiCenter.h"

class Logic {
public:
	/*
	 * The function constructs a new Logic object.
	 */
	Logic();
	/*
	 * The function destructs a Logic object.
	 */
	virtual ~Logic();
	/*
	 * The function returns the grid.
	 */
	Grid* getGrid();
	/*
	 * The function returns the taxi center.
	 */
	TaxiCenter* getCenter();
private:
	Grid* grid;
	TaxiCenter* center;
};

#endif /* LOGIC_H_ */
