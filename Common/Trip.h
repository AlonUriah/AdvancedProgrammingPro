/*
 * This is the trip class. It handles
 * a trip which is done by a source and destination
 * points, a driver and a taxi.
 */#include "Point.h"
#include "Driver.h"
#include "Taxi.h"
#include "../SearchAlgo/BFS.h"
#include "Scheduled.h"
#include "Grid.h"
#include "Node.h"
#include "../SearchAlgo/Searchable.h"

#ifndef TRIP_H_
#define TRIP_H_

#include "Common.h"

class Trip : public Scheduled {
private:
	int id;
	Point source;
	Point destination;
	Point currentLocation;
	Driver* driver;
	Status status;
	int passengers;
	double tariff;
	int startTime;
	Grid* map;
	stack<Searchable*>* route;
	pthread_mutex_t bfs_locker;
public:
	/*
	 * Constructor
	 */
	Trip(int id, Point src, Point dest, int passengers, double tariff, int startTime, Grid* map);
	/*
	 * Copy constructor
	 */
	Trip(const Trip& trip);
	/*
	 * Assign driver to this trip
	 */
	void assignDriver(Driver* driver);
	/*
	 * Gets the status of the trip
	 */
	Status getStatus();
	/*
	 * Gets the trip id
	 */
	int getId();
	/*
	 * Get source
	 */
	Point& getSource();
	/*
	 * Get start time
	 */
	int getTime();
	/*
	 * Get destination
	 */
	Point& getDestination();
	/*
	 * Get location
	 */
	Point& getCurrentLocation();
	/*
	 * Calculate its route
	 */
	void calculateRoute();
	/*
	 * Scheduled abstract method
	 */
	void timePassed(int time);
	/*
	 * Assignment operator overloading
	 */
	Trip& operator = (const Trip &trip);
	/*
	 * Two trips are equal iff their value is the same
	 */
	bool operator == (const Trip &trip) const;
	/*
	 * Trip destructor
	 */
	virtual ~Trip();
};

#endif /* TRIP_H_ */

/*
 * Flow:
 * Trip is being created by TaxiCenter.
 * All of the arguments passed to constructor are ELEMANTRY to create a trip.
 *
 * Trip should know answering	: What is your current location
 * 								: What is your status
 * 								: How long until you get customer?
 * 				 				: number of passengers (hoover?)
 *								: AbortTrip
 *								: Calculate new root (abort/mission changed)
 */
