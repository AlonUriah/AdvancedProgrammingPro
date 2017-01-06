/*
 * Passenger.cpp
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */

#include "Passenger.h"
/*
 * Constructs an empty passenger.
 */
Passenger::Passenger() {
	this->source = new Point(5,5);
	this->destination = new Point(3,4);
}
/*
 * Constructs a passenger with a src point,
 * And a destination point.
 */
Passenger::Passenger(Point* src, Point* dest)
{
	this->source = src;
	this->destination = dest;
}
/*
 * Destructs the current passenger.
 */
Passenger::~Passenger() {

}
Point* Passenger::getDestination()
{
	return this->destination;
}
Point* Passenger::getSource()
{
	return this->source;
}
