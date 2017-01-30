/*
 * AcceptClientTask.h
 * This Task handles the following:
 * Receiving driver from client and sent him a
 * Taxi back
 */

#include "../ITask.h"
#include "../../Common/Driver.h"
#include "../../Common/Taxi.h"
#include "../../Common/Logger.h"
#include "../../Common/Common.h"
#include "../../Common/ClientData.h"

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

#ifndef MULTITHREADING_TASKS_ACCEPTCLIENTTASK_H_
#define MULTITHREADING_TASKS_ACCEPTCLIENTTASK_H_

class AcceptClientTask : public ITask{
private:
	ClientData* _clientData;
	list<Driver*>* _drivers;
	list<Taxi*>* _cabs;
	pthread_mutex_t* _driversLocker;
	int _serverSocket;

	/*
	 * Receives serialized driver and returns
	 * a deserialized driver.
	 */
	Driver* receiveDriver();

	/*
	 * Scans cab list and assign a taxi to a given driver
	 */
	Taxi* assignTaxi(Driver* driver);

	/*
	 * Serialize a given taxi and sends it to client
	 */
	void sendTaxiToClient(Taxi* taxi);

	/*
	 * Adds a given driver to driver list
	 */
	void updateDriversList(Driver* driver);

public:
	/*
	 * Constructor
	 */
	AcceptClientTask(int clientId,  ClientData* args,
			int serverSocket, list<Driver*>* drivers, pthread_mutex_t* driversLocker,
			list<Taxi*>* cabs);

	/*
	 * Receives driver from client, assign it a taxi
	 * and call sendTaxiToClient.
	 * Updates drivers list.
	 */
	virtual void start();

	/*
	 * Destructor
	 */
	virtual ~AcceptClientTask();
};

#endif /* MULTITHREADING_TASKS_ACCEPTCLIENTTASK_H_ */
