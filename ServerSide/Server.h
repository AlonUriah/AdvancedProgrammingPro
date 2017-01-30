/*
 * Server.h
 *
 *  Created on: Jan 28, 2017
 *      Author: alon
 */

#include <list>
#include <pthread.h>
#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


#include "../Common/ClientData.h"
#include "../Common/Logger.h"
#include "../Common/Taxi.h"
#include "../Common/Driver.h"
#include "../Common/Point.h"
#include "../Multithreading/ThreadPool.h"


#ifndef SERVERSIDE_SERVER_H_
#define SERVERSIDE_SERVER_H_

using namespace std;

class Server {
private:
	int _port;
	struct sockaddr_in _serverAddr;
	int _serverSocket;
	pthread_mutex_t _clientsLocker;

	void initServer();
	virtual Point& getDriversLocation(int id) = 0;
	virtual Taxi* addDriver(Driver* driver) = 0;

protected:
	Logger* _logger;
	list<ClientData*>* _clients = new list<ClientData*>;

public:
	Server(int port);
	void addClients(int num, list<Taxi*>* cabs, list<Driver*>* drivers,
			pthread_mutex_t& driversLocker, ThreadPool* handler);
	void broadcast();
	void abort();
	virtual ~Server();
};

#endif /* SERVERSIDE_SERVER_H_ */
