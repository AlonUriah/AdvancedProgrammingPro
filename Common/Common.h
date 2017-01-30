//============================================================================
// Name        : Common.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Commonly used functions will be implemented in here
//============================================================================
#include <vector>
#include <string>
#include <sstream>
#include "./Factories/GridFactory.h"
#include "Obstacle.h"

#ifndef HEADERS_COMMON_H_
#define HEADERS_COMMON_H_

using namespace std;

enum Marital {SINGLE, MARRIED, DIVORCED, WIDOWED};
enum Status {CALCULATING, PENDING, READY, TO_CUSTOMER,TO_DESTINATION,TO_CENTER, ENDED};
enum Color {RED, BLUE, GREEN, PINK, WHITE};
enum Manufacturer {HONDA, SUBARU, TESLA, FIAT};
enum Conn_Status{SUCCESS,FAILED};
enum Logger_Levels{DEBUG,INFO,WARN};
const string Logger_Levels_Str[] = {"DEBUG","INFO","WARN"};

struct DriverWrapper{
	int id;
	int age;
	char status;
	int xp;
	int vehicle_id;
};

struct TaxiWrapper{
	int id;
	int type;
	char manu;
	char color;
};

struct TripWrapper{
	int id;
	int startX;
	int startY;
	int endX;
	int endY;
	int passengers;
	double tariff;
	int startTime;
};

/*
 * Splits an input 's' by a given delimeter 'delim'.
 * Used an example online.
 */
vector<string> split(const string& s, char delim);

/*
 * Parses user input.
 * Returns a vector of strings separated by ','.
 */
vector<string> parseInput();

/*
 * Parses Grid input
 * Should get Grid's x, y, number of obstacles and their locations.
 */
GridFactory* parseGridInfo();

/*
 * Parses Obstacles input.
 * First reads a number describing how many obstacles to expect.
 * Create obstacles accordingly.
 * Add them to the map by swapping the current default node in location
 * obstacleLocation to obstacle.
 * Returns an integer describing how many obstacles were created.
 */
int parseObstacles(Grid* map);

/*
 * Parses driver info by the following sequence:
 * id,age,status,experience and vehicle id.
 * Returns a pointer to a struct of 'DriverWrapper'
 * that contains all the relevant data to generate
 * a new Driver.
 */
DriverWrapper* parseDriver();

/*
 * Parses taxi info by the following sequence:
 * id,type,manufacture and color.
 * Returns a pointer to a struct of 'TaxiWrapper'
 * that contains all the relevant data to generate a new
 * Taxi.
 */
TaxiWrapper* parseTaxi();

/*
 * Parses trip info by the following sequence:
 * id,startX,startY,endX,endY,passengers number and tariff.
 * Returns a pointer to a struct of 'TripWrapper'
 * that contains all the relevant data to generate
 * a new Trip.
 */
TripWrapper* parseTrip(Grid* map, int time);
/*
 * Checks for string, if it contains only
 * numbers and a special character
 */
bool numberValidation(string input, char special);
#endif /* HEADERS_COMMON_H_ */
