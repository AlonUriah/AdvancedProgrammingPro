/*
 * This is the trip class. It handles
 * a trip which is done by a source and destination
 * points, a driver and a taxi.
 */
#include "Trip.h"
#include <stack>
#include "../SearchAlgo/Searchable.h"

Trip::Trip(int id, Point src, Point dest, int passengers, double tariff, int startTime, Grid* map)
{
	this->tariff = tariff;
	this->source = src;
	this->destination = dest;
	this->currentLocation = src;
	this->status = CALCULATING;
	this->passengers = passengers;
	this->id = id;
	this->startTime = startTime;
	// Unresolved details, yet.
	this->driver = NULL;
	this->map = map;
	this->route = NULL;//new stack<Searchable*>;

	pthread_mutex_init(&this->bfs_locker, 0);
}
/*
 * Assign a driver to this trip
 */
void Trip::assignDriver(Driver* driver)
{
	this->driver = driver;
	this->driver->setAvailability(false);
	this->status = READY;
}
/*
 * Gets the trip id
 */
int Trip::getId()
{
	return this->id;
}
/*
 * Gets the status of the trip
 */
Status Trip::getStatus()
{
	return this->status;
}

void Trip::setStatus(Status status){
	this->status = status;
}

/*
 * Get source point
 */
Point& Trip::getSource()
{
	return this->source;
}
/*
 * Get start time
 */
int Trip::getTime()
{
	return this->startTime;
}
/*
 * Get destination point
 */
Point& Trip::getDestination()
{
	return this->destination;
}
/*
 * Get current location
 */
Point& Trip::getCurrentLocation()
{
	return this->currentLocation;
}
/*
 * Calculate is route
 */
bool Trip::calculateRoute()
{
	// Build the nodes according to src and dest
	Node begin(this->source);
	Node end(this->destination);

	// Get the route
	pthread_mutex_lock(&this->bfs_locker);
	this->route = BFS::getRoot(*this->map, &begin, &end);
	pthread_mutex_unlock(&this->bfs_locker);

	// Check if there is a route
	if (this->route->empty())
		return false;

	// Pop first point
	this->route->pop();
	return true;

}

void* Trip::getRouteAddress(){
	return route;
}

/*
 * Time has passed by 1 hour,
 * now lets check what is the current
 * time, and decide what to do with the trip.
 */
void Trip::timePassed(int time)
{
	if (this->startTime == time && this->status == READY)
	{
		this->status = TO_DESTINATION;
	}
	else if (this->startTime < time && this->status == TO_DESTINATION)
	{
		// Advance the taxi by speed
		// Pop out one move.
		if (!this->route->empty())
		{
			Searchable* sr = this->route->top();
			this->currentLocation = sr->getValue();
			this->route->pop();
			sr = NULL;
			// If it's a luxurytaxi, pop out another move.
			if (this->driver->getSpeed() == 2 && !this->route->empty())
			{
				this->currentLocation = this->route->top()->getValue();
				this->route->pop();
			}
			// Update driver's location
			this->driver->updateLocation(this->currentLocation);
		}

		if (this->currentLocation == this->destination)
		{
			this->status = ENDED;
			this->driver->setAvailability(true);
			return;
		}

	}
}
/*
 * Copy constructor
 */
Trip::Trip(const Trip& trip)
{
	source = trip.source;
	destination = trip.destination;
	currentLocation = trip.currentLocation;
	passengers = trip.passengers;
	status = trip.status;
	tariff = trip.tariff;
	id = trip.id;
	startTime = trip.startTime;

	driver = NULL;

	if (trip.driver != NULL)
		driver = trip.driver;
	route = trip.route;
	map = trip.map;
}
/*
 * Assignment operator overloading
 */
Trip& Trip::operator = (const Trip &trip)
{
	if(this == &trip){
		return *this;
	}
	source = trip.source;
	destination = trip.destination;
	currentLocation = trip.currentLocation;
	passengers = trip.passengers;
	status = trip.status;
	tariff = trip.tariff;
	id = trip.id;
	startTime = trip.startTime;
	driver = NULL;
	route = trip.route;
	if (trip.driver != NULL)
		driver = trip.driver;

	map = trip.map;
	return *this;
}

void Trip::assignRoute(stack<Searchable*>* root){
	route = root;
}

/*
 * Two trips are equal iff their value is the same
 */
bool Trip::operator == (const Trip &trip) const
{
	if(id == trip.id){
		return true;
	}
	return false;
}

int Trip::getExecutionTime(){
	return startTime;
}

/*
 * Destructor
 */
Trip::~Trip()
{
	delete this->route;
	pthread_mutex_destroy(&this->bfs_locker);
	this->map = NULL;
	this->driver = NULL;
}
