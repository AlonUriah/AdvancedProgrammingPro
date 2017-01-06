/*
 * Passenger.h
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */

#ifndef PASSENGER_H_
#define PASSENGER_H_

#include "Point.h"
#include "Driver.h"
#include <iostream>

class Passenger {
public:
	/*
	 * Constructs an empty passenger.
	 */
	Passenger();
	/*
	 * Constructs a passenger with a src point,
	 * And a destination point.
	 */
	Passenger(Point* src, Point* dest);
	/*
	 * Destructs the current passenger.
	 */
	virtual ~Passenger();
	Point* getSource();
	Point* getDestination();
private:
	Point* source;
	Point* destination;
};

#endif /* PASSENGER_H_ */
