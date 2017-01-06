/*
 * This is the TaxiCenter class.
 * It handles a taxi center, and give
 * the functionality of adding taxies,
 * drivers and trips, and handling them.
 */
#include "TaxiCenter.h"

/*
 * Constructs a new taxi center.
 */
TaxiCenter::TaxiCenter() {
	// Initialize the lists.
	this->cabs = new list<Taxi*>;
	this->drivers = new list<Driver*>;
	this->rides = new list<Trip*>;
	this->clock = new Clock();
	this->clock->addListener(this);
	/*
	 * Parses users input and return a grid factory.
	 * This way our code could support multiple Grids
	 */
	this->factory = parseGridInfo();
	this->map = this->factory->create();
	/*
	 * Read info for obstacles handling.
	 * First parameter is the number of obstacles to expect.
	 * Later on, for each obstacle capture its location using
	 * the following format <x value><space><y value>
	 */
	parseObstacles(map);
}
/*
 * Destructs the taxi center
 */
TaxiCenter::~TaxiCenter() {
	Taxi* taxi = NULL;
	Driver* driver = NULL;
	Trip* trip = NULL;

	// Free the trips' list
	while (!this->rides->empty())
	{
		trip = this->rides->front();
		this->rides->pop_front();
		delete trip;
	}
	trip = NULL;
	delete this->rides;

	// Free the drivers' list
	while (!this->drivers->empty())
	{
		driver = this->drivers->front();
		this->drivers->pop_front();
		delete driver;
	}
	driver = NULL;
	delete this->drivers;

	// Free the cabs' list.
	while (!this->cabs->empty())
	{
		taxi = this->cabs->front();
		this->cabs->pop_front();
		delete taxi;
	}
	taxi = NULL;
	delete this->cabs;

	this->clock->removeListener(this);
	delete this->clock;
	delete this->map;
	delete this->factory;
}
/*
 * Adds a new driver
 */
Taxi* TaxiCenter::addDriver(Driver* d)
{
	/*
	 * If there's a taxi which its id is the same
	 * as the driver's vehicle's id, we attach this
	 * taxi to the driver.
	 */
	Taxi* taxi = NULL;
	for (list<Taxi*>::iterator it = this->cabs->begin(); it != this->cabs->end(); ++it)
	{
		if ((*it)->getId() == d->getTaxiId())
		{
			d->assignTaxi(*it);
			taxi = *it;
			break;
		}
	}
	// Push the driver to the list.
	this->drivers->push_back(d);
	return taxi;
}
/*
 * Adds a new taxi.
 */
void TaxiCenter::addTaxi(int id, int type, char manu, char color)
{
	// Create a taxi according to the parameters.
	Taxi* taxi;
	switch (type)
	{
		// Regular taxi type.
		case 1:
			taxi = new RegularTaxi(id, manu, color);
			this->cabs->push_back(taxi);
			break;
		// Luxury taxi type.
		case 2:
			taxi = new LuxuryTaxi(id, manu, color);
			this->cabs->push_back(taxi);
			break;
	}

	// Assign the taxi to the correct driver (by id).
	for (list<Driver*>::iterator it = this->drivers->begin(); it != this->drivers->end(); ++it)
	{
		if ((*it)->getTaxiId() == id)
		{
			(*it)->assignTaxi(taxi);
			break;
		}
	}
	taxi = NULL;
}
/*
 * Adds a new ride
 */
void TaxiCenter::addRide(int id, int startX, int startY,
						 int endX, int endY, int passengers, double tariff, int startTime)
{
	// Initialize two points according to the parameters.
	Point start(startX, startY);
	Point end(endX, endY);
	// Creates a new trip.
	Trip* trip = new Trip(id, start, end, passengers, tariff, startTime, this->map);
	// Push the trip to the list.
	this->rides->push_back(trip);
	// Add the trip to listen to the clock
	this->clock->addListener(trip);
}
/*
 * Gets a driver's location
 */
Point& TaxiCenter::getDriversLocation(int id)
{
	/*
	 * Iterating through the drivers' list,
	 * and if there's a proper id, retrieve
	 * its current location.
	 */
	for (list<Driver*>::iterator it = this->drivers->begin(); it != this->drivers->end(); ++it)
	{
		if ((*it)->getId() == id)
			return (*it)->getLocation();
	}
	throw;
}
/*
 * Assign the rides to the drivers,
 * and moving the drivers to the end point.
 */
void TaxiCenter::timePassed(int time)
{
	// Declare two iterators.
	list<Driver*>::iterator driversIt = this->drivers->begin();
	list<Trip*>::iterator tripsIt = this->rides->begin();
	/*
	 * For each trip, assign a driver, according
	 * to the availability.
	 */
	while (tripsIt != this->rides->end()
		&& driversIt != this->drivers->end())
	{
		if ((*tripsIt)->getStatus() == PENDING
			&& (*tripsIt)->getTime() == time)
		{
			(*tripsIt)->assignDriver(*driversIt);
			++driversIt;
		}
		++tripsIt;
	}
	// After assigning drivers, start driving.
}
/*
 * Advances time by 1.
 */
void TaxiCenter::advanceTime()
{
	Trip* trip;
	this->clock->notifyAll();
	// Clean ended trips
	list<Trip*>::iterator tripsIt = this->rides->begin();
	while (tripsIt != this->rides->end())
	{
		if ((*tripsIt)->getStatus() == ENDED)
		{
			this->clock->removeListener(*tripsIt);
			trip = *tripsIt;
			tripsIt = this->rides->erase(tripsIt);
			delete trip;
		}
		else
			++tripsIt;
	}
}
