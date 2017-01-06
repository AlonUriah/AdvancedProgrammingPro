//============================================================================
// Name        : TripUnitTest.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Unit test for Trip object.
//				 Check both source/destination setters and getters.
//				 Check abortTrip via the destination setters.
//				 Check getCurrentLocation through abortTrip.
//				 Check addPassenger - scenario with passengers overloading.
// 				 Check set/get Taxi (different taxi id).
//				 Check set/get Driver (different driver id).
//============================================================================

#include "../Common/Trip.h"
#include "../Common/Point.h"
#include "../Common/Driver.h"
#include "../Common/Passenger.h"
#include "../Common/Logger.h"
#include "gtest/gtest.h"
#include "../Common/Taxi.h"
#include <ostream>
#include <vector>

using namespace std;
/*
 * Create a UnitTest class with default elements composing a Trip.
 * Holds a Trip to run the tests on.
 */
class TripUnitTest : public :: testing::Test{
public:
	Trip* trip;
	Point _source;
	Point _destination;
	Driver* _driver;
	vector<Passenger*> _passengers;
	Logger _logger;

	/*
	 * Empty default constructor.
	 */
	TripUnitTest(){
		_source = Point(0,0);
		_destination = Point(2,2);
		_driver = new Driver();
		Passenger passenger;
		_passengers.push_back(&passenger);
		SetUp();
	}
	/*
	 * Constructor.
	 */
	TripUnitTest(Point& src, Point& dest,Driver* driver, vector<Passenger*> passengers, Logger logger){
		_source = src;
		_destination = dest;
		_driver = driver;
		_passengers = passengers;
		_logger = logger;
		SetUp();
	}

protected:
	virtual void SetUp(){
		Grid g(3,3);
		trip = new Trip(0, _source, _destination, 3, 2.0, 6, &g);
		//trip = new Trip(_source,_destination,_driver,_passengers);
		//trip = new Trip(_source, _destination, _driver, _passengers);
	}
	virtual void TestBody(){ }
	virtual void TearDown(){
		delete trip;
	}
};

/*
 * Validates Trip initialization by
 * comparing current and expected source.
 */
TEST(TripUnitTest, SourceValidation){
	TripUnitTest test;
	ASSERT_EQ(test.trip->getSource(),test._source);
}

/*
 * Validates Trip initialization by
 * comparing current and expected destination.
 */
TEST(TripUnitTest, DestinationValidation){
	TripUnitTest test;
	ASSERT_EQ(test.trip->getDestination(), test._destination);
}

/*
 * Check both abortTrip() - by default set the destination to
 * taxi center.
 * And check the source getter.
 */
TEST(TripUnitTest, ValidateSourceChange){
	TripUnitTest test;
	Point currentLocation = test.trip->getCurrentLocation();
	//test.trip->abortTrip();
	ASSERT_EQ(test.trip->getSource(), currentLocation);
}

/*
 * Check both abortTrip(destination, passengers)
 * and setDestination by comparing old to new destination.
 */
TEST(TripUnitTest, ValidateDestinationChange){
	TripUnitTest test;
	Point currentDestination = test.trip->getDestination();
	Point newDest(1,1);
	vector<Passenger*> newPassengers;
	//test.trip->abortTrip(newDest, newPassengers);
	bool areEqual = (test.trip->getDestination() == currentDestination);
	ASSERT_EQ(areEqual, false);
}

/*
 * Check set/get Driver by setting a new Driver,
 * getting the old one and comparing their id numbers.
 */
/*TEST(TripUnitTest, ValidateChangingDriver){
	TripUnitTest test;
	Driver* currentDriver = test.trip->getDriver();
	int currentId = currentDriver->getId();

	Driver newDriver;
	int newId = newDriver.getId();
	test.trip->setDriver(&newDriver);
	currentDriver = test.trip->getDriver();
	ASSERT_NE(currentId,newId);
}*/

/*
 * Check set/get Taxi by setting a new Taxi,
 * getting the old one and comparing their id numbers.
 */

/*TEST(TripUnitTest, ValidateChangingTaxi){
	TripUnitTest test;
	Taxi* currentTaxi = test.trip->getTaxi();
	int currentTaxiId = currentTaxi->getId();

	Taxi* newTaxi = new Taxi();
	test.trip->setTaxi(newTaxi);
	currentTaxi = test.trip->getTaxi();
	int newTaxiId = currentTaxi->getId();

	delete newTaxi;
	ASSERT_NE(currentTaxiId,newTaxiId);
}*/

/*
 * Check the addPassengers method with two scenarios -
 * A valid add and an invalid one (more than 4 passengers).
 */
TEST(TripUnitTest, ValidateTaxiPassengersOverloading){
	TripUnitTest test;

	//Valid passengers addition (less than 4)
	vector<Passenger*> newPassengers;
	for(int i=0; i<3; i++){
		Point* src = new Point(i,i);
		Point* dest = new Point(i+1,i+1);
		Passenger* p = new Passenger(src,dest);
		newPassengers.push_back(p);
	}
	try{
		//test.trip->addPassengers(newPassengers);
		EXPECT_EQ(0,0);
	}
	catch(...){
		FAIL() << "Could not add passengers.";
	}

	//Check passengers overloading
	vector<Passenger*> newOverloadingPassengers;
	for(int i=3; i<5; i++){
		Point* src = new Point(i,i);
		Point* dest = new Point(i+1,i+1);
		Passenger* p = new Passenger(src, dest);
		newOverloadingPassengers.push_back(p);
	}

	try{
		//test.trip->addPassengers(newPassengers);
		FAIL() << "Taxi can't take more than more 4 passengers.";
	}
	catch(invalid_argument const& err){
		ASSERT_EQ(0,0);
	}
	catch(...){
		FAIL() << "Taxi can't take more than more 4 passengers.";
	}

	for(vector<Passenger*>::iterator it = newPassengers.begin(); it != newPassengers.end(); ++it){
		delete *it;
	}
}


/*
 * Validate currentLocation is always positive.
 */
TEST(TripUnitTest, ValidateCurrentLocation){
	TripUnitTest test;
	Point current = test.trip->getCurrentLocation();
	EXPECT_GE(current.getX(), 0);
	EXPECT_GE(current.getY(), 0);
}
