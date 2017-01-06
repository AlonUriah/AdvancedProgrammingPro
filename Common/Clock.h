/*
 * Clock.h
 *
 *  Created on: Dec 30, 2016
 *      Author: uriah
 */
#include <list>
#include <iostream>
#include "Scheduled.h"

using namespace std;

#ifndef CLOCK_H_
#define CLOCK_H_
/*
 * The clock class handles a clock,
 * timed from 0, and each time it gets
 * and input, it increases by 1.
 */
class Clock {
public:
	/*
	 * A Clock constructor
	 */
	Clock();
	/*
	 * Clock destructor
	 */
	virtual ~Clock();
	/*
	 * The method notifies all the observers,
	 * that the time has passed.
	 */
	void notifyAll();
	/*
	 * Adds a listener to the Scheduled items' list.
	 */
	void addListener(Scheduled* scheduled);
	/*
	 * Removes a listener from the Scheduled items' list.
	 */
	void removeListener(Scheduled* scheduled);
	/*
	 * Returns the current time.
	 */
	int getTime();
private:
	int time;
	list<Scheduled*>* listeners;
};

#endif /* CLOCK_H_ */