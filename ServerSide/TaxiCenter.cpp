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
TaxiCenter::TaxiCenter(int port) : Server(port) {
	// Initialize the lists.
	this->cabs = new list<Taxi*>;
	this->drivers = new list<Driver*>;
	this->rides = new list<Trip*>;
	this->clock = new Clock();
	this->clock->addListener(this);

	this->threads_num = 15;

	pthread_mutex_init(&this->drivers_locker, 0);
	pthread_mutex_init(&this->rides_locker, 0);
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
	/*
	 * Start reading input from the user.
	 */
	this->input();
}
/*
 * Destructs the taxi center
 */
TaxiCenter::~TaxiCenter()
{
	Taxi* taxi = NULL;
	Driver* driver = NULL;
	Trip* trip = NULL;

	pthread_mutex_destroy(&this->rides_locker);
	pthread_mutex_destroy(&this->drivers_locker);

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
	pthread_mutex_lock(&this->drivers_locker);
	this->drivers->push_back(d);
	pthread_mutex_unlock(&this->drivers_locker);

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
void TaxiCenter::addRide(TripWrapper* tripWrapper)
{
	// Initialize two points according to the parameters.
	Point start(tripWrapper->startX, tripWrapper->startY);
	Point end(tripWrapper->endX, tripWrapper->endY);
	// Creates a new trip.
	Trip* trip = new Trip(tripWrapper->id,
						  start,
						  end,
						  tripWrapper->passengers,
						  tripWrapper->tariff,
						  tripWrapper->startTime,
						  this->map);

	// Calculate the route
	trip->calculateRoute();

	// Push the trip to the list.
	pthread_mutex_lock(&this->rides_locker);
	this->rides->push_back(trip);
	pthread_mutex_unlock(&this->rides_locker);

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
/*
 * The method handles the input from the user.
 * for each number, there is a specific action.
 */
void TaxiCenter::input()
{
	// Declarations.
	TripWrapper* tripWrapper = NULL;
	TaxiWrapper* taxiWrapper = NULL;
	TripInfo* tripInfo = NULL;
	int id;
	int numOfDrivers;
	int userChoice;
	string input;

	pthread_t threadsPool[this->threads_num];
	int thread_counter = 0;

	// Input loop.
	do
	{
		// Input a number.
		cin >> userChoice;

		switch(userChoice){
		// Input drivers
			case 1:
				cin >> numOfDrivers;
				this->handleClients(numOfDrivers);
				break;
		// Input a trip.
			case 2:
				tripWrapper = parseTrip();
				tripInfo = new TripInfo();
				tripInfo->tripWrapper = tripWrapper;
				tripInfo->taxiCenter = this;
				pthread_create(&threadsPool[thread_counter], NULL, callAddRide, (void*)tripInfo);
				delete tripWrapper;
				break;
		// Input a taxi.
			case 3:
				taxiWrapper = parseTaxi();
				this->addTaxi(taxiWrapper->id,taxiWrapper->type,
									      taxiWrapper->manu,taxiWrapper->color);
				delete taxiWrapper;
				break;
		// Retrieve a location of a driver.
			case 4:
				cin >> id;
				cout << this->getDriversLocation(id) << endl;
				break;
		// Abort the program.
			case 7:
				this->Abort();
				break;
		// Move the time forward by 1.
			case 9:
				// Check if calculations has finished
				for (int i = 0; i < this->threads_num; i++)
					pthread_join(threadsPool[i], NULL);

				this->advanceTime();
				this->broadcast();
				break;
			default:
				break;
		}
	} while (userChoice != 7);
}
/*
 * Call addRide function for thread
 */
void* TaxiCenter::callAddRide(void* element)
{
	TripInfo* tripInfo = (TripInfo*)element;
	tripInfo->taxiCenter->addRide(tripInfo->tripWrapper);
	tripInfo->taxiCenter = NULL;
	return NULL;
}

