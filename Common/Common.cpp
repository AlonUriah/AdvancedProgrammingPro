//============================================================================
// Name        : Common.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Commonly used functions will be implemented in here
//============================================================================

#include "Common.h"
#include "./Factories/GridFactory.h"
#include "Obstacle.h"
#include <iostream>

using namespace std;

/*
 * Splits an input 's' by a given delimeter 'delim'.
 * Used an example online.
 */
vector<string> split(const string& s, char delim){
	vector<string> splitted;
	splitted.clear();
	stringstream stringStream;
	stringStream.str(s);
	string item;
	while(getline(stringStream, item, delim)){
		splitted.push_back(item);
	}
	stringStream.clear();
	return splitted;
}

/*
 * Parses user input.
 * Returns a vector of strings separated by ','.
 */
vector<string> parseInput(){
	string input;
	cin >> input;
	vector<string> commaSplitted;
	commaSplitted = split(input, ',');
	return commaSplitted;
}

/*
 * Parses Grid input
 * Should get Grid's x, y, number of obstacles and their locations.
 */
GridFactory* parseGridInfo(){
	string input;
	getline(cin, input);

	// Validate numbers only
	if (!numberValidation(input, ' '))
		return NULL;

	// Split the input by spaces
	vector<string> spaceSplitted;
	spaceSplitted = split(input, ' ');

	// Map size check
	if (spaceSplitted.size() != 2)
		return NULL;

	// Try parse to integer - x axis
	int xValue, yValue;
	stringstream s;
	try
	{
		s << spaceSplitted[0];
		s >> xValue;
		s.clear();
		s << spaceSplitted[1];
		s >> yValue;
	}
	catch (...)
	{
		return NULL;
	}

	// Validation check
	if (xValue <= 0 || yValue <= 0)
		return NULL;

	GridFactory* factory = new GridFactory(xValue,yValue);
	return factory;
}

/*
 * Parses Obstacles input.
 * First reads a number describing how many obstacles to expect.
 * Create obstacles accordingly.
 * Add them to the map by swapping the current default node in location
 * obstacleLocation to obstacle.
 * Returns an integer describing how many obstacles were created.
 */
int parseObstacles(Grid* map){
	stringstream stringStream;
	string input;

	//Read how many expected obstacles are there
	getline(cin, input);
	int numOfObstacles;
	stringStream << input;

	// Number validation check
	try
	{
		stringStream >> numOfObstacles;
	}
	catch (...)
	{
		return -1;
	}

	// Validation Check
	if (numOfObstacles < 0)
		return -1;

	//Get ready to capture their location
	int x;
	int y;

	for(int i = 0; i < numOfObstacles; i++)
	{
		stringStream.clear();

		//Read and parse obstacle axis info
		getline(cin, input);
		vector<string> pointXY = split(input, ',');

		// Two arguments validation
		if (pointXY.size() != 2)
			return -1;

		// Numbers validation
		try
		{
			stringStream << pointXY[0];
			stringStream >> x;
			stringStream.clear();
			stringStream << pointXY[1];
			stringStream >> y;
		}
		catch (...)
		{
			return -1;
		}

		// Validation Check
		if (x < 0 || y < 0 || x >= map->getWidth() || y >= map->getHeight())
			return -1;

		// Create a new obstacle based on that info
		Point obstacleLocation(x,y);
		Obstacle* currentObstacle = new Obstacle(obstacleLocation);
		map->swapNodes(obstacleLocation, currentObstacle);
	}

	return numOfObstacles;
}


/*
 * Parses driver info by the following sequence:
 * id,age,status,experience and vehicle id.
 * Returns a pointer to a struct of 'DriverWrapper'
 * that contains all the relevant data to generate
 * a new Driver.
 */
DriverWrapper* parseDriver(){
	stringstream stringStream;
	vector<string> splittedInfo;
	// Capture input that was splitted by ',' in parseInput
	splittedInfo = parseInput();
	int id;
	int age;
	char status;
	int xp;
	int vehicle_id;

	stringStream.clear();
	stringStream << splittedInfo[0];
	stringStream >> id;

	stringStream.clear();
	stringStream << splittedInfo[1];
	stringStream >> age;

	stringStream.clear();
	stringStream << splittedInfo[2];
	stringStream >> status;

	stringStream.clear();
	stringStream << splittedInfo[3];
	stringStream >> xp;

	stringStream.clear();
	stringStream << splittedInfo[4];
	stringStream >> vehicle_id;

	// Create a return value of a DriverWrapper.
	DriverWrapper* wrapper = new DriverWrapper;
	wrapper->id = id;
	wrapper->age = age;
	wrapper->status = status;
	wrapper->xp = xp;
	wrapper->vehicle_id = vehicle_id;

	return wrapper;
}

/*
 * Parses taxi info by the following sequence:
 * id,type,manufacture and color.
 * Returns a pointer to a struct of 'TaxiWrapper'
 * that contains all the relevant data to generate a new
 * Taxi.
 */
TaxiWrapper* parseTaxi(){
	stringstream stringStream;
	vector<string> splittedInfo;
	splittedInfo = parseInput();

	// Validation check
	if (splittedInfo.size() != 4)
		return NULL;

	for (int i = 0; i < 2; i++)
		if (!numberValidation(splittedInfo[i], '1'))
			return NULL;

	for (int j = 2; j < 4; j++)
		if (splittedInfo[j].size() != 1)
			return NULL;

	int id;
	int type;
	char manu;
	char color;

	stringStream.clear();
	stringStream << splittedInfo[0];
	stringStream >> id;

	if (id < 0)
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[1];
	stringStream >> type;

	if (type != 1 && type != 2)
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[2];
	stringStream >> manu;

	if (manu != 'H' && manu != 'S' && manu != 'T' && manu != 'F')
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[3];
	stringStream >> color;

	if (color != 'R' && color != 'B' && color != 'G' && color != 'P' && color != 'W')
		return NULL;

	// Store info in a return value of type struct TaxiWrapper.
	TaxiWrapper* taxiWrapper = new TaxiWrapper;
	taxiWrapper->id = id;
	taxiWrapper->type = type;
	taxiWrapper->manu = manu;
	taxiWrapper->color = color;

	return taxiWrapper;
}

/*
 * Parses trip info by the following sequence:
 * id,startX,startY,endX,endY,passengers number and tariff.
 * Returns a pointer to a struct of 'TripWrapper'
 * that contains all the relevant data to generate
 * a new Trip.
 */
TripWrapper* parseTrip(Grid* map, int time)
{
	stringstream stringStream;
	vector<string> splittedInfo;
	splittedInfo = parseInput();

	// Validation check
	if (splittedInfo.size() != 8)
		return NULL;

	// Validate numbers only
	for (int i = 0; i < splittedInfo.size(); i++)
		if (splittedInfo[i].size() == 0
			|| !numberValidation(splittedInfo[i], '1'))
			return NULL;

	int id;
	int startX;
	int startY;
	int endX;
	int endY;
	int passengers;
	double tariff;
	int startTime;

	stringStream.clear();
	stringStream << splittedInfo[0];
	stringStream >> id;

	// Validate negative id
	if (id < 0)
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[1];
	stringStream >> startX;

	// Coordinates inside map
	if (startX < 0 || startX >= map->getWidth())
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[2];
	stringStream >> startY;

	if (startY < 0 || startY >= map->getHeight())
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[3];
	stringStream >> endX;

	if (endX < 0 || endX >= map->getWidth())
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[4];
	stringStream >> endY;

	if (endY < 0 || endY >= map->getHeight())
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[5];
	stringStream >> passengers;

	if (passengers <= 0)
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[6];
	stringStream >> tariff;

	if (tariff <= 0)
		return NULL;

	stringStream.clear();
	stringStream << splittedInfo[7];
	stringStream >> startTime;

	if (startTime <= time)
		return NULL;

	// Store info in a return value of type struct TripWrapper.
	TripWrapper* tripWrapper = new TripWrapper;
	tripWrapper->id = id;
	tripWrapper->startX = startX;
	tripWrapper->startY = startY;
	tripWrapper->endX = endX;
	tripWrapper->endY = endY;
	tripWrapper->passengers = passengers;
	tripWrapper->tariff = tariff;
	tripWrapper->startTime = startTime;

	return tripWrapper;
}
/*
 * Checks for string, if it contains only
 * numbers and a special character
 */
bool numberValidation(string input, char special)
{
	// First char should be a number
	if (input.at(0) < '0' || input.at(0) > '9')
		return false;
	// Validate that only numbers are shown in
	for (int i = 0; i < input.length(); i++)
		if ((input.at(i) < '0' || input.at(i) > '9') && input.at(i) != special)
			return false;
	return true;
}
