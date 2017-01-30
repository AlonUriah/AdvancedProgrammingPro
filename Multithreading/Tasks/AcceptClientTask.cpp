//============================================================================
// Name        : AcceptClientTask.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : This Task handles the client accepting
//				 It sends the client a Taxi and receives
//				 a driver
//============================================================================

#include "AcceptClientTask.h"
#include "../ITask.h"
#include "../../Common/Logger.h"
#include "../../Common/Driver.h"
#include "../../Common/Taxi.h"
#include "../../ServerSide/Server.h"

#include <list>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <pthread.h>

using namespace std;

/*
 * Constructor
 */
AcceptClientTask::AcceptClientTask(int clientId,  ClientData* args, int serverSocket,
	 list<Driver*>* drivers, pthread_mutex_t* driversLocker,
	 list<Taxi*>* cabs) : ITask(clientId, args){
	_clientData = args;
	_serverSocket = serverSocket;
	_drivers = drivers;
	_cabs = cabs;
	_driversLocker = driversLocker;
}

/*
 * Receives driver from client, assign it a taxi
 * and call sendTaxiToClient.
 * Updates drivers list.
 */
void AcceptClientTask::start(){
	Taxi* taxi = NULL;
	Driver* driver = NULL;
	Logger* logger = Logger::getInstance();

	logger->info("Start handling client...");

	driver = receiveDriver();

	// If data was not sent/received properly, abort this task
	if(driver == NULL){
		return;
	}

	// Assign taxi to driver from cabs list
	taxi = assignTaxi(driver);

	if (taxi == NULL)
	{
		delete driver;
		return;
	}

	sendTaxiToClient(taxi);
	updateDriversList(driver);
}

/*
 * Receives serialized driver and returns
 * a deserialized driver.
 */
Driver* AcceptClientTask::receiveDriver(){
	Driver* driver = NULL;

	// Declare a buffer, and reset it
	char buffer[4096];
	memset(buffer, 0, sizeof(buffer));

	// Get the Driver from the current client
	long bytes = recv(_clientData->clientVal, buffer, sizeof(buffer), 0);

	if (bytes <= 0)
	{
		_logger->warn("Could not get any data from client.");
		return NULL;
	}

	stringstream bufferStream;
	bufferStream << buffer;
	driver = new Driver();
	{
		boost::archive::text_iarchive ia(bufferStream);
		ia >> *driver;
	}

	// Set the client id, to be the driver's one
	_clientData->driverId = driver->getId();
	_logger->info("Driver received.");

	return driver;
}

/*
 * Scans cab list and assign a taxi to a given driver
 */
Taxi* AcceptClientTask::assignTaxi(Driver* driver){
	Taxi* taxi = NULL;

	for (list<Taxi*>::iterator it = _cabs->begin();
			it != _cabs->end(); ++it)
	{
		if ((*it)->getId() == driver->getTaxiId())
		{
			driver->assignTaxi(*it);
			taxi = *it;
			break;
		}
	}

	return taxi;
}

/*
 * Serialize a given taxi and sends it to client
 */
void AcceptClientTask::sendTaxiToClient(Taxi* taxi){
	stringstream taxiStream;
	boost::archive::text_oarchive oa(taxiStream);
	oa << *taxi;

	const string tmp = taxiStream.str();
	const char* data = tmp.c_str();
	int data_len = strlen(data);

	int sent_bytes;
	try
	{
		sent_bytes = send(_clientData->clientVal, data, data_len, 0);
	}
	catch (...)
	{
		// Close the socket.
		close(_serverSocket);
		taxi = NULL;
	}

	if (sent_bytes < 0)
	{
		_logger->warn("Taxi cannot be sent.");
		return;
	}

	_logger->info("Taxi was sent.");
}

/*
 * Adds a given driver to driver list
 */
void AcceptClientTask::updateDriversList(Driver* driver){
	pthread_mutex_lock(_driversLocker);
	_drivers->push_back(driver);
	pthread_mutex_unlock(_driversLocker);

	_logger->info("Driver was added");
}

/*
 * Destructor
 */
AcceptClientTask::~AcceptClientTask() {
	_clientData = 0;
	_drivers = 0;
	_cabs = 0;
	_driversLocker = 0;
}

