/*
 * Logic.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */

#include "Logic.h"
#include  <iostream>
/*
 * The function constructs a new Logic object.
 */
Logic::Logic() {
	this->center = NULL;
	this->grid = NULL;
}
/*
 * The function destructs a Logic object.
 */
Logic::~Logic() {

}
/*
 * The function returns the grid.
 */
Grid* Logic::getGrid()
{
	return this->grid;
}
/*
 * The function returns the taxi center.
 */
TaxiCenter* Logic::getCenter()
{
	return this->center;
}

