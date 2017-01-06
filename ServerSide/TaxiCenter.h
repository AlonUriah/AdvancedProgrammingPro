/*
 * This is the TaxiCenter class.
 * It handles a taxi center, and give
 * the functionality of adding taxies,
 * drivers and trips, and handling them.
 */
#include "../Common/Factories/GridFactory.h"
#include "../Common/Grid.h"
#include "../Common/Clock.h"
#include "../Common/Trip.h"
#include <iostream>
#include "../Common/LuxuryTaxi.h"
#include "../Common/RegularTaxi.h"
#include "../Common/Common.h"
#include <list>
#ifndef TAXICENTER_H_
#define TAXICENTER_H_

using namespace std;

class TaxiCenter : public Scheduled {
public:
	/*
	 * Constructs a new taxi center.
	 */
	TaxiCenter();
	/*
	 * Destrcuts the taxi center
	 */
	virtual ~TaxiCenter();
	/*
	 * Adds a new driver
	 */
	void addDriver(int id, int age, char status, int xp, int vehicle_id);
	Taxi* addDriver(Driver* d);
	/*
	 * Adds a new taxi.
	 */
	void addTaxi(int id, int type, char manu, char color);
	/*
	 * Adds a new ride.
	 */
	void addRide(int id, int startX, int startY,
				 int endX, int endY, int passengers, double tariff, int startTime);
	/*
	 * Gets a driver's location
	 */
	Point& getDriversLocation(int id);
	/*
	 * Assign the rides to the drivers,
	 * and moving the drivers to the end point.
	 */
	void execute();

	/*
	 * Driving the trips
	 */
	void startDriving();
	void timePassed(int time);
	/*
	 * Advances time by 1.
	 */
	void advanceTime();
private:
	list<Driver*>* drivers;
	list<Taxi*>* cabs;
	list<Trip*>* rides;
	Clock* clock;
	Grid* map;
	GridFactory* factory;
};

#endif /* TAXICENTER_H_ */
