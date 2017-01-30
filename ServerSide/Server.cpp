/*
 * Server.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: alon
 */

#include <pthread.h>
#include <list>

#include "Server.h"
#include "../Common/ClientData.h"
#include "../Multithreading/ThreadPool.h"
#include "../Multithreading/Tasks/AcceptClientTask.h"

using namespace std;

Server::Server(int port) {
	_port = port;
	_serverSocket = -1;
	_clients = new list<ClientData*>;

	// Init clients list locker
	pthread_mutex_init(&_clientsLocker,0);

	// Get the logger singleton
	_logger = Logger::getInstance();
	_logger->shouldPrint(true);
	_logger->info("SERVER Initialized.");
}

void Server::initServer(){
	_logger->info("Initializing socket...");

	// Open a listening socket, TCP type.
	_serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1){
		_logger->warn("Socket was not able to create.");
	}

	_logger->info("Setting server details...");

	// Define the server details.
	memset(&_serverAddr, 0, sizeof(_serverAddr));
	_serverAddr.sin_family = PF_INET;
	_serverAddr.sin_addr.s_addr = INADDR_ANY;
	_serverAddr.sin_port = htons(_port);
	bzero(&(_serverAddr.sin_zero), 8);

	_logger->info("Binding the port...");

		// Try to bind the port.
	int bindRet = ::bind(_serverSocket,
					(struct sockaddr*)&_serverAddr,
					sizeof(struct sockaddr));

	if (bindRet < 0)
	{
		_logger->warn("Port was not able to be binded.");
		return;
	}

	_logger->info("Server is now listening.");

	// Start listening to broadband
	listen(_serverSocket, 5);
}

void Server::addClients(int num, list<Taxi*>* cabs, list<Driver*>* drivers,
		pthread_mutex_t& driversLocker, ThreadPool* handler){
	// Start server in case it was not already started
	if (_serverSocket < 0)
	{
		try{
			initServer();
		}
		catch (...)
		{
			_logger->info("An error occurred. Server is closing...");

			// Close the socket if there's an error.
			close(_serverSocket);

			_logger->info("Server is closed.");
			return;
		}
	}

	/*
	 * Start receiving clients
	 */
	try{
		int clientsReceived = 0;
		while (clientsReceived < num)
		{
			// Declare on client's data to preserve

			ClientData* currentClient;
			int client, clientVal;
			unsigned int client_len = sizeof(client);

			// Accept a new client
			clientVal = accept(_serverSocket, (struct sockaddr*)&client, &client_len);
			if (clientVal < 0)
			{
				//_logger->warn("Client was unable to connect.");
				continue;
			}

			_logger->info("Client accepted.");

			// Create a new client data type, and store the data
			currentClient = new ClientData();
			currentClient->client = client;
			currentClient->clientVal = clientVal;
			currentClient->clientLen = client_len;

			ITask* handleClientTask = new AcceptClientTask(clientsReceived,
															currentClient,
															_serverSocket,
															drivers,
															&driversLocker,
															cabs);
			handler->addTask(handleClientTask);

			_clients->push_back(currentClient);
			clientsReceived++;
		}

		/*
		 * Wait for all the clients to finish the registration
		 * process, of sending a 'Driver' object, and getting
		 * back a 'Taxi' object.
		 */
		_logger->info("Finished handling clients.");
		return;
	} catch (...){
		/*
		 * If the connection has lost, we can
		 * close it, and return.
		 */
		close(_serverSocket);
		return;
	}
}

void Server::broadcast(){
	_logger->info("Broadcasting points to clients.");

	/*
	 * Iterate each client data, and send them the
	 * next point they should be at.
	 */
	list<ClientData*>::iterator it = _clients->begin();
	while (it != _clients->end())
	{
		// Get the point by the template design pattern
		Point p1(getDriversLocation((*it)->driverId));
		// Serialize the point.
		stringstream pointStream;
		boost::archive::text_oarchive oa(pointStream);
		oa << p1;

		const string tmp = pointStream.str();
		const char* data = tmp.c_str();
		int data_len = strlen(data);
		// Send the point to the client.
		try
		{
			int sent_bytes = send((*it)->clientVal, data, data_len, 0);
			if (sent_bytes < 0) {
				_logger->warn("Point was not able to be sent.");
			}
			else
			{
				_logger->info("Point was sent.");
			}
			}
			catch (...)
			{
				_logger->warn("An error occurred. Point couldn't not be sent.");
			}
			++it;
	}
}

void Server::abort()
{
	_logger->info("Server is closing...");

	// Send the client '7' to notify the server is going to close.
	for (list<ClientData*>::iterator it = _clients->begin(); it != _clients->end(); ++it)
	{
		try
		{
			int sent_bytes = send((*it)->clientVal, "7", 1, 0);
			if (sent_bytes < 0) {
				_logger->info("Exit sign was not able to be sent.");
			}
		}
		catch (...)
		{
			_logger->info("Exit sign was not able to be sent.");
		}
	}

	// Close the server.
	close(_serverSocket);
	_logger->info("Server is closed.");

}

Server::~Server() {
	// Destroy the mutex
	pthread_mutex_destroy(&_clientsLocker);
	// Delete the clients' data
	ClientData* currentClient = NULL;


	while (!_clients->empty())
	{
		currentClient = _clients->front();
		_clients->pop_front();
		delete currentClient;
	}

	delete _clients;
	_logger->info("SERVER Destroyed.");
	delete _logger;
}

