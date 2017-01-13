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

	// Split the input by spaces
	vector<string> spaceSplitted;
	spaceSplitted = split(input, ' ');

	// Try parse to integer - x axis
	stringstream stringStream;
	int xValue, yValue;
	stringStream << spaceSplitted[0];
	stringStream >> xValue;

	// Try parse to integer - y axis
	stringStream.clear();
	stringStream << spaceSplitted[1];
	stringStream >> yValue;

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
	stringStream >> numOfObstacles;

	//Get ready to capture their location
	int x;
	int y;

	for(int i=0; i<numOfObstacles; i++){
		stringStream.clear();

		//Read and parse obstacle axis info
		getline(cin, input);
		vector<string> pointXY = split(input,',');
		stringStream << pointXY[0];

		stringStream >> x;

		stringStream.clear();
		stringStream << pointXY[1];
		stringStream >> y;

		//Create a new obstacle based on that info
		Point obstacleLocation(x,y);
		Obstacle* currentObstacle = new Obstacle(obstacleLocation);
		map->swapNodes(obstacleLocation,currentObstacle);
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

	int id;
	int type;
	char manu;
	char color;

	stringStream.clear();
	stringStream << splittedInfo[0];
	stringStream >> id;

	stringStream.clear();
	stringStream << splittedInfo[1];
	stringStream >> type;

	stringStream.clear();
	stringStream << splittedInfo[2];
	stringStream >> manu;

	stringStream.clear();
	stringStream << splittedInfo[3];
	stringStream >> color;

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
TripWrapper* parseTrip(){
	stringstream stringStream;
	vector<string> splittedInfo;
	splittedInfo = parseInput();

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

	stringStream.clear();
	stringStream << splittedInfo[1];
	stringStream >> startX;

	stringStream.clear();
	stringStream << splittedInfo[2];
	stringStream >> startY;

	stringStream.clear();
	stringStream << splittedInfo[3];
	stringStream >> endX;

	stringStream.clear();
	stringStream << splittedInfo[4];
	stringStream >> endY;

	stringStream.clear();
	stringStream << splittedInfo[5];
	stringStream >> passengers;

	stringStream.clear();
	stringStream << splittedInfo[6];
	stringStream >> tariff;

	stringStream.clear();
	stringStream << splittedInfo[7];
	stringStream >> startTime;

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
