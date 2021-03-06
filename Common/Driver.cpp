/*
 * This is the driver class.
 * The class handles a driver.
 */
#include "Common.h"
#include "Driver.h"
/*
 * Destructs the current driver.
 */
Driver::~Driver() {
	// Avoiding dangling pointer
	this->taxi = 0;
}
/*
 * Constructs a new driver.
 */
Driver::Driver(int id, int age, char status, int xp, int vehicle_id)
{
	// Initializing properties.
	this->id = id;
	this->age = age;
	this->xp = xp;
	this->vehicle_id = vehicle_id;
	this->location = Point(0, 0);
	this->available = true;
	// Assigning a status to the driver.
	switch (status)
	{
		case 's':
		case 'S':
			this->status = SINGLE;
			break;
		case 'd':
		case 'D':
			this->status = DIVORCED;
			break;
		case 'w':
		case 'W':
			this->status = WIDOWED;
			break;
		case 'm':
		case 'M':
			this->status = MARRIED;
			break;
		default:
			this->status = SINGLE;
	}
	// Taxi is not assigned yet.
	this->taxi = NULL;
}
/*
 * Copy constructor
 */
Driver::Driver(const Driver& driver)
{
	id = driver.id;
	age = driver.age;
	status = driver.status;
	xp = driver.xp;
	vehicle_id = driver.vehicle_id;
	available = driver.available;
	taxi = NULL;
	if (driver.taxi != NULL)
		taxi = driver.taxi;
}
/*
 * Returns the id.
 */
int Driver::getId()
{
	return this->id;
}
/*
 * Get age
 */
int Driver::getAge()
{
	return this->age;
}
/*
 * Get status
 */
Marital Driver::getStatus()
{
	return this->status;
}
/*
 * Get experience
 */
int Driver::getXp()
{
	return this->xp;
}
/*
 * Gets taxi's id.
 */
int Driver::getTaxiId()
{
	return this->vehicle_id;
}
/*
 * Get the driver's taxi's speed
 */
int Driver::getSpeed()
{
	int speed = -1;
	if (this->taxi != NULL)
		speed = this->taxi->getSpeed();
	return speed;
}
/*
 * Retrieves the drivers' availability
 * to handle a new trip
 */
bool Driver::isAvailable()
{
	return this->available;
}
/*
 * Assigns a taxi to this driver
 */
void Driver::assignTaxi(Taxi* taxi)
{
	this->taxi = taxi;
}
/*
 * Gets driver's location
 */
Point& Driver::getLocation()
{
	return this->location;
}
/*
 * Updates the location of the driver
 */
void Driver::updateLocation(Point& location)
{
	this->location = location;
}
/*
 * Operator overloading.
 */
Driver& Driver::operator = (const Driver &driver)
{
	if (this == &driver)
		return *this;
	id = driver.id;
	age = driver.age;
	status = driver.status;
	xp = driver.xp;
	available = driver.available;
	vehicle_id = driver.vehicle_id;
	//taxi = driver.taxi;
	return *this;
}
/*
 * Operator overloading.
 */
bool Driver::operator == (const Driver &driver) const
{
	if(id == driver.id){
		return true;
	}
	return false;
}
/*
 * Get the driver's taxi
 */
Taxi* Driver::getTaxi(){
	return this->taxi;
}
/*
 * Set the availability of
 * the driver.
 */
void Driver::setAvailability(bool val)
{
	this->available = val;
}
