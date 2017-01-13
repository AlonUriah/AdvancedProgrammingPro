/*
 * This is the Server class.
 * It handles the whole server
 * functionalities, including the input
 * from the server manager, and send/receive,
 * inputs from the clients.
 * Moreover, it handles the TaxiCenter, which
 * manages the whole data.
 */




#ifndef SERVER_H_
#define SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Common.h"
#include "Driver.h"
#include "Taxi.h"
#include <list>
#include <stdexcept>
#include <pthread.h>

using namespace std;
/*
 * This is the Server class.
 * It handles the whole server
 * functionalities, including the input
 * from the server manager, and send/receive,
 * inputs from the clients.
 * Moreover, it handles the TaxiCenter, which
 * manages the whole data.
 */
class Server {
public:
	/*
	 * A server constructor.
	 */
	Server(int port);
	/*
	 * Server destructor
	 */
	virtual ~Server();
	/*
	 * The method start a listening connection.
	 * In the future, it will listen as a different
	 * Thread, while each incoming connection will be
	 * followed to another Thread.
	 */
	void handleClients(int num);
	/*
	 * The method gets a Point, and send
	 * it to the client. This is for trailing
	 * its route.
	 */
	void broadcast();
	/*
	 * The method aborts the connection.
	 * It happens while sending the character
	 * '7' as an input.
	 */
	void Abort();
private:
	/*
	 * A private struct which organizes
	 * the data of a client, which we need
	 * in order to communicate with.
	 */
	struct ClientData {
		int client;
		unsigned int client_len;
		int driverId;
		Server* server;
	};

	int port;
	struct sockaddr_in server_addr;
	int serverSocket;
	list<ClientData*>* clients;
	pthread_mutex_t clients_locker;
	/*
	 * Handle a specific client, in a separate thread
	 */
	void* handleClient(void* element);
	/*
	 * Calling the handleClient method.
	 * This is a static method, since the pthread_create
	 * function receives a specific pointer to function,
	 * and it can't be an inner method class one, because
	 * the 'this' reference is hidden among the
	 * method parameters.
	 */
	static void* callHandleClient(void* element);
	/*
	 * Finds a drivers location.
	 *
	 * Using the template design pattern.
	 * We remain it as an abstract method,
	 * and wish for the children to implement
	 * it.
	 */
	virtual Point& getDriversLocation(int id) = 0;
	/*
	 * Adds a driver to the list, and gets
	 * back a 'Taxi' object which belongs
	 * to the driver.
	 *
	 * Using the template design pattern.
	 * We remain it as an abstract method,
	 * and wish for the children to implement
	 * it.
	 */
	virtual Taxi* addDriver(Driver* driver) = 0;
};

#endif /* SERVER_H_ */
