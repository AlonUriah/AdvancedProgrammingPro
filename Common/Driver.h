/*
 * This is the driver class.
 * The class handles a driver.
 */


//Ex3 add-ons


using namespace std;
#ifndef DRIVER_H_
#define DRIVER_H_

#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Common.h"
#include "Point.h"
#include "Taxi.h"
#include "RegularTaxi.h"
#include "LuxuryTaxi.h"
#include <iostream>

class Driver {
private:
	int id;
	int age;
	int xp;
	int vehicle_id;
	bool available;
	Point location;
	Taxi* taxi;
	Marital status;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version){
		archive & BOOST_SERIALIZATION_NVP(id);
		archive & BOOST_SERIALIZATION_NVP(age);
		archive & BOOST_SERIALIZATION_NVP(vehicle_id);
		archive & BOOST_SERIALIZATION_NVP(location);
		archive & BOOST_SERIALIZATION_NVP(taxi);
		archive & BOOST_SERIALIZATION_NVP(status);
	}

public:
	string serializeString;
	Driver(){
		id = -1;
		age = -1;
		xp = -1;
		vehicle_id = -1;
		location = Point(0,0);
		taxi = 0;
		status = WIDOWED;
		available = true;
	};
	/*
	 * Copy constructor
	 */
	Driver(const Driver& driver);
	/*
	 * Constructs a new driver.
	 */
	Driver(int id, int age, char status, int xp, int vehicle_id);
	/*
	 * Destructs the current driver.
	 */
	virtual ~Driver();
	/*
	 * Returns the id.
	 */
	int getId();
	/*
	 * Get age
	 */
	int getAge();
	/*
	 * Get status
	 */
	Marital getStatus();
	/*
	 * Get experience
	 */
	int getXp();
	/*
	 * Get taxi id
	 */
	int getTaxiId();
	/*
	 * Assigning a taxi to the driver
	 */
	void assignTaxi(Taxi* taxi);
	/*
	 * Gets driver's location
	 */
	Point& getLocation();
	/*
	 * Updates the location of the driver
	 */
	void updateLocation(Point& location);
	/*
	 * Get the driver's taxi's speed
	 */
	int getSpeed();
	/*
	 * Set the availability of
	 * the driver.
	 */
	void setAvailability(bool val);
	/*
	 * Retrieves the drivers' availability
	 * to handle a new trip
	 */
	bool isAvailable();
	/*
	 * Operator overloading
	 */
	Driver& operator = (const Driver &driver);
	/*
	 * Operator overloading
	 */
	bool operator == (const Driver &driver) const;
	
	//Client purposes
	Taxi* getTaxi();

	// For gTest purposes
	int getSatisfaction();
	void setExperience(int exp);
	void setSatisfaction(int satisfaction);
	void setStatus(Marital s);
};

#endif /* DRIVER_H_ */
