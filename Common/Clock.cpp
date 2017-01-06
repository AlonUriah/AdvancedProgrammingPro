/*
 * The clock class handles a clock,
 * timed from 0, and each time it gets
 * and input, it increases by 1.
 */
#include "Clock.h"

/*
 * A Clock constructor
 */
Clock::Clock() {
	// Zero to time
	this->time = 0;
	// Initialize a list of Scheduled items.
	this->listeners = new list<Scheduled*>;
}
/*
 * The method notifies all the observers,
 * that the time has passed.
 */
void Clock::notifyAll()
{
	// Increases the time by 1.
	++this->time;
	// For each observer, notify him the time has passed.
	for (list<Scheduled*>::iterator it = this->listeners->begin();
			it != this->listeners->end(); ++it)
		(*it)->timePassed(this->time);
}
/*
 * Adds a listener to the Scheduled items' list.
 */
void Clock::addListener(Scheduled* scheduled)
{
	this->listeners->push_back(scheduled);
}
/*
 * Removes a listener from the Scheduled items' list.
 */
void Clock::removeListener(Scheduled* scheduled)
{
	/*
	 * Since for now, each Scheduled item is actually
	 * a Trip, we already handle its destruction in TaxiCenter.
	 * So for now, we only erase the pointer, and not delete
	 * the object itself, to avoid dangling pointers and
	 * memory leaks.
	 */
	list<Scheduled*>::iterator it = this->listeners->begin();
	while (it != this->listeners->end())
		if ((*it) == scheduled)
		{
			it = this->listeners->erase(it);
			break;
		}
		else
			++it;
}
/*
 * Returns the current time.
 */
int Clock::getTime()
{
	return this->time;
}
/*
 * Clock destructor
 */
Clock::~Clock() {
	// Empty the list.
	while (!this->listeners->empty())
		this->listeners->pop_front();
	// Delete it.
	delete this->listeners;
}
