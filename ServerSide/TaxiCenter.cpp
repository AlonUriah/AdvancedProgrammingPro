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
	int retVal = -1;
	// Initialize the lists.
	this->cabs = new list<Taxi*>;
	this->drivers = new list<Driver*>;
	this->rides = new list<Trip*>;
	this->threadsPool = new list<pthread_t>;

	// Initialize the clock
	this->clock = new Clock();
	// Add the taxi center to listen to the clock
	this->clock->addListener(this);

	// Initialize two mutexes
	pthread_mutex_init(&this->drivers_locker, 0);
	pthread_mutex_init(&this->rides_locker, 0);
	pthread_mutex_init(&this->bfs_locker, 0);
	/*
	 * Parses users input and return a grid factory.
	 * This way our code could support multiple Grids
	 */
	this->factory = NULL;
	while (retVal < 0)
	{
		/*
		 * While the factory hasn't got
		 * a valid input, keep inputing it.
		 */
		while (this->factory == NULL)
			this->factory = parseGridInfo();
		// Create the map
		this->map = this->factory->create();
		/*
		 * Read info for obstacles handling.
		 * First parameter is the number of obstacles to expect.
		 * Later on, for each obstacle capture its location using
		 * the following format <x value><space><y value>
		 */
		retVal = parseObstacles(this->map);
		/*
		 * If the obstacles input is not valid,
		 * delete the map, and start the input
		 * from the beginning.
		 */
		if (retVal < 0)
		{
			delete this->map;
			delete this->factory;
			this->map = NULL;
			this->factory = NULL;
		}
	}
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
	// Declare pointing items
	Taxi* taxi = NULL;
	Driver* driver = NULL;
	Trip* trip = NULL;

	// Destroy mutexes
	pthread_mutex_destroy(&this->bfs_locker);
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

	// Remove the taxi center from the listener's list
	this->clock->removeListener(this);

	// Delete lists/map
	delete this->cabs;
	delete this->threadsPool;
	delete this->clock;
	delete this->map;
	delete this->factory;

	this->logger->info("Taxi center was deleted.");

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

	this->logger->info("Driver was added.");

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

	this->logger->info("Taxi was added.");

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
	pthread_mutex_lock(&this->bfs_locker);
	bool retVal = trip->calculateRoute();
	pthread_mutex_unlock(&this->bfs_locker);

	if (!retVal)
	{
		delete trip;
		return;
	}
	// Push the trip to the list.
	pthread_mutex_lock(&this->rides_locker);
	this->rides->push_back(trip);
	pthread_mutex_unlock(&this->rides_locker);

	// Add the trip to listen to the clock
	this->clock->addListener(trip);

	this->logger->info("Trip was added.");

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
	list<Driver*>::iterator driversIt;
	list<Trip*>::iterator tripsIt = this->rides->begin();
	/*
	 * For each trip, assign a driver, according
	 * to the availability.
	 */
	while (tripsIt != this->rides->end())
	{
		driversIt = this->drivers->begin();
		while (driversIt != this->drivers->end())
		{
			if ((*tripsIt)->getStatus() == PENDING
				&& (*tripsIt)->getTime() == time
				&& (*driversIt)->isAvailable()
				&& (*driversIt)->getLocation() == (*tripsIt)->getSource())
			{
				(*tripsIt)->assignDriver(*driversIt);
				break;
			}
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
	// Notify all listeners, that time has passed
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

	stringstream ss;
	ss << "Time is now: " << this->clock->getTime() << ".";
	this->logger->info(ss.str());

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
	int driversNum;
	int userChoice;
	string input, numOfDrivers, driversId;
	stringstream numDrivers, driversIdParser;
	list<pthread_t>::iterator it;

	// Input loop.
	do
	{
		// Input a number.
		cin >> userChoice;

		switch(userChoice)
		{
		// Input drivers
			case 1:
				try
				{
					numDrivers.clear();
					cin >> numOfDrivers;
					// Validation check
					if (!numberValidation(numOfDrivers, '1'))
					{
						cout << "-1" << endl;
						continue;
					}
					numDrivers << numOfDrivers;
					numDrivers >> driversNum;
					this->handleClients(driversNum);
				}
				catch (...)
				{
					cout << "-1" << endl;
					continue;
				}
				break;
		// Input a trip.
			case 2:
				// Declare a new thread
				pthread_t currentThread;
				// Get the input from the user
				tripWrapper = parseTrip(this->map, this->clock->getTime());
				// Validation check
				if (tripWrapper == NULL)
				{
					cout << "-1" << endl;
					continue;
				}
				// Organize the struct for the thread function
				tripInfo = new TripInfo();
				tripInfo->tripWrapper = tripWrapper;
				tripInfo->taxiCenter = this;
				// Create the thread
				pthread_create(&currentThread, NULL, callAddRide, (void*)tripInfo);
				// Add it to the threads pool
				this->threadsPool->push_back(currentThread);
				break;
		// Input a taxi.
			case 3:
				taxiWrapper = parseTaxi();
				if (taxiWrapper == NULL)
				{
					cout << "-1" << endl;
					continue;
				}
				this->addTaxi(taxiWrapper->id,taxiWrapper->type,
									      taxiWrapper->manu,taxiWrapper->color);
				delete taxiWrapper;
				break;
		// Retrieve a location of a driver.
			case 4:
				try
				{
					cin >> driversId;
					if (!numberValidation(driversId, '1'))
					{
						cout << "-1" << endl;
						continue;
					}
					driversIdParser << driversId;
					driversIdParser >> id;
				}
				catch (...)
				{
					cout << "-1" << endl;
					continue;
				}
				cout << this->getDriversLocation(id) << endl;
				break;
		// Abort the program.
			case 7:
				this->Abort();
				break;
		// Move the time forward by 1.
			case 9:
				// Check if calculations of trips' routes has finished
				it = this->threadsPool->begin();
				/*
				 * Iterate the list of threads,
				 * for each one, join it, and wait
				 * until it's finished.
				 * After that, remove it from the list.
				 */
				while (!this->threadsPool->empty())
				{
					pthread_join(*it, NULL);
					it = this->threadsPool->erase(it);
				}
				// Advance time by 1
				this->advanceTime();
				// Broadcast to all clients the current location
				this->broadcast();
				break;
			default:
				cout << "-1" << endl;
				break;
		}
	} while (userChoice != 7);
}
/*
 * Call addRide function for thread
 */
void* TaxiCenter::callAddRide(void* element)
{
	// Cast the client data
	TripInfo* tripInfo = (TripInfo*)element;
	// Call the addRide method to add a trip
	tripInfo->taxiCenter->addRide(tripInfo->tripWrapper);
	// Avoid dangling pointer
	tripInfo->taxiCenter = NULL;
	// Delete the structs
	delete tripInfo->tripWrapper;
	delete tripInfo;
	return NULL;
}

