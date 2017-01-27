//============================================================================
// Name        : ClientMain.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Client's Main Flow (Stopped at any point of a connection failing):
// Try to connect -> Get a driver from user -> send driver to server ->
// get taxi from server and assign it to driver ->
// start receiving location updates
//============================================================================
// Members references
#include "Client.h"
#include "../Common/Common.h"
#include "../Common/Driver.h"
#include "../Common/Taxi.h"
#include "../Common/Logger.h"

// Serialization references
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <string.h>

using namespace std;

// Constants
const int SUCCESS_RETURN_CODE = 1;
const int FAIL_RETURN_CODE = 0;

/*
 * Client's Main Flow (Stopped at any point of a connection failing):
 * Try to connect -> Get a driver from user -> send driver to server ->
 * get taxi from server and assign it to driver ->
 * start receiving location updates
 */
int main(int argc, char** argv){

	// Get Driver from user
	DriverWrapper* driverWrapper = parseDriver();

	// Validation check
	if (driverWrapper == NULL)
		exit(1);

	Driver* driver = new Driver(	driverWrapper->id,
					driverWrapper->age,
					driverWrapper->status,
					driverWrapper->xp,
					driverWrapper->vehicle_id);


	//Get Logger's singletone
	Logger* logger = Logger::getInstance();
	logger->debug("Client main thread was started...");

	char* serverIpAdd = (char*)"127.0.0.1";
	int port;
	bool isAlive = true;

	// Convert port string input into integer
	stringstream portConverter;
	portConverter << argv[2];
	portConverter >> port;

	// Assign argv[1] to serverIpAdd (Readability)
	if(strcmp(argv[1],"localhost") != 0){
		serverIpAdd = argv[1];
	}

	// Initiate a program's client
	Client client(serverIpAdd,port);

	// Try connect to server, exit program, safely, at each FAIL
	if(client.initConnection() == FAILED){
		isAlive = false;
		client.disconnect();
		logger->warn("Could not connect server, please try again later");
		return FAIL_RETURN_CODE;
	}



	client.setDriver(driver);

	// Serialize driver into a string
	stringstream driverStream;
	boost::archive::text_oarchive driverOutArchive(driverStream);
	driverOutArchive << *driver;

	// Save string to a constant char array
	const string serializedDriverStr = driverStream.str();
	const char* data = serializedDriverStr.c_str();

	// Send to server
	int data_len = strlen(data);
	if(client.sendMsg(data,data_len) == FAILED){
		isAlive = false;
		client.disconnect();
		logger -> warn("Could not send driver to server, about to exit the program");

		return FAIL_RETURN_CODE;
	}

	// Once a driver was sent, expect server to send a Taxi to assign to it
	if(client.receiveTaxi() == FAILED){
		isAlive = false;
		client.disconnect();
		logger -> warn("Could not get taxi from server, about to exit the program");

		return FAIL_RETURN_CODE;
	}

	// Start client's listening
	if(client.startReceiving() == FAILED){
		isAlive = false;
		client.disconnect();
		logger -> warn("Connection was interrupted, about to exit the program");

		return FAIL_RETURN_CODE;
	}

	// Try to close connection safely
	if(isAlive){
		try{
			client.disconnect();

			logger->debug("Connection was determined successfully");
		}catch(...){

			logger->warn("Failed to determine connection");
		}
	}

	// Free resources
	Taxi* taxi = driver->getTaxi();
	if(taxi != 0){
		delete taxi;
	}
	if(driver != 0){
		delete driver;
	}
	delete driverWrapper;
	delete logger;

	return SUCCESS_RETURN_CODE;
}
