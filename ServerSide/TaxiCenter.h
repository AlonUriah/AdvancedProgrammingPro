/*
 * This is the TaxiCenter class.
 * It handles a taxi center, and give
 * the functionality of adding taxies,
 * drivers and trips, and handling them.
 */


#ifndef TAXICENTER_H_
#define TAXICENTER_H_

#include "GridFactory.h"
#include "Grid.h"
#include "Clock.h"
#include "Trip.h"
#include <iostream>
#include "LuxuryTaxi.h"
#include "RegularTaxi.h"
#include "Taxi.h"
#include "Common.h"
#include "Scheduled.h"
#include "Server.h"
#include <list>
using namespace std;


class TaxiCenter : public Scheduled, public Server {
private:
	int threads_num;

	list<Driver*>* drivers;
	list<Taxi*>* cabs;
	list<Trip*>* rides;
	Clock* clock;

	Grid* map;
	GridFactory* factory;
	pthread_mutex_t drivers_locker;
	pthread_mutex_t rides_locker;

	struct TripInfo
	{
		TripWrapper* tripWrapper;
		TaxiCenter* taxiCenter;
	};

public:

	/*
	 * Constructs a new taxi center.
	 */
	TaxiCenter(int port);
	/*
	 * Destrcuts the taxi center
	 */
	virtual ~TaxiCenter();
	/*
	 * Adds a new driver
	 */
	Taxi* addDriver(Driver* d);
	/*
	 * Adds a new taxi.
	 */
	void addTaxi(int id, int type, char manu, char color);
	/*
	 * Adds a new ride.
	 */
	void addRide(TripWrapper* tripWrapper);
	/*
	 * Gets a driver's location
	 */
	Point& getDriversLocation(int id);
	/*
	 * Assign the rides to the drivers,
	 * and moving the drivers to the end point.
	 */
	void timePassed(int time);
	/*
	 * Advances time by 1.
	 */
	void advanceTime();
	/*
	 * The method handles the input from the user.
	 * for each number, there is a specific action.
	 */
	void input();
	/*
	 * Call the addRide function to thread
	 */
	static void* callAddRide(void* element);

};

#endif /* TAXICENTER_H_ */
