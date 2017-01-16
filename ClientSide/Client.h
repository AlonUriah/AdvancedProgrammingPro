//============================================================================
// Name        : Client.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : This Client class is responsible
//				 for client rolling in this exercise.
//				 Among its feature you can find a connection oriented methods
//				 such as - connect, disconnet, receive and send.
//				 And one startReceiving which acts like user
//============================================================================
// Members
#include "../Common/Common.h"
#include "../Common/Driver.h"
#include "../Common/Logger.h"

// Connection Members
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef CLIENT_H_
#define CLIENT_H_

class Client {
private:
	char* _serverIpAddress;
	int _serverPortNumber;
	int _socket;
	struct sockaddr_in _sin;
	Driver* _driver;
	Logger* _logger;

public:
	/*
	 * Default Client constructor
	 */
	Client();

	/*
	 * Constructor
	 * Sets Clients _serverIpAddress and _serverPortNumner members.
	 * _socket and driver setting is decoupled from constructor logic.
	 */
	Client(char* serverAddress, int serverPortNumber);

	/*
	 * Connect to a server at _serverAdrress through _serverPortNumber
	 * This method uses an UDP connection (SOCK_DGRAM).
	 */
	Conn_Status initConnection();

	/*
	 * Disconnects client from server.
	 * Announce user and return a proper Conn_Status.
	 */
	Conn_Status disconnect();

	/*
	 * Sends one message (data[]) to server.
	 * Announce user via appropriate Conn_Status
	 */
	Conn_Status sendMsg(const char data[], int data_len);

	/*
	 * Receive ONE message from server.
	 * This method will be called once to get driver's Taxi.
	 * From that point the client will be listening constantly.
	 */
	Conn_Status receiveTaxi();

	/*
	 * Set Client's driver member
	 */
	void setDriver(Driver* driver);

	/*
	 * Starts the constant looping of receiving data from server
	 */
	Conn_Status startReceiving();

	/*
	 * Client's destructor
	 * Avoid dangling pointer
	 */
	~Client();
};

#endif /* CLIENT_H_ */
