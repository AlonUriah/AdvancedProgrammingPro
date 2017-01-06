//============================================================================
// Name        : Client.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : This Client class is responsible
//				 for client rolling in this exercise.
//				 Among its feature you can find a connection oriented methods
//				 such as - connect, disconnet, receive and send.
//				 And one startReceiving which acts like user
//============================================================================

// Members references
#include "Client.h"
#include "../Common/Taxi.h"
#include "../Common/Common.h"
#include <cstring>

// Connection references
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>

// Serialization references
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;

/*
 * Default Client constructor
 */
Client::Client(){
	_serverIpAddress = (char*)"";
	_serverPortNumber = -1;
	_socket = 0;
	_driver = 0;
}

/*
 * Constructor
 * Sets Clients _serverIpAddress and _serverPortNumner members.
 * _socket and driver setting is decoupled from constructor logic.
 */
Client::Client(char* serverAddress, int serverPortNumber) {
	_serverIpAddress = serverAddress;
	_serverPortNumber = serverPortNumber;
	_socket = 0;
	_driver = 0;
}

/*
 * Connect to a server at _serverAdrress through _serverPortNumber
 * This method uses an UDP connection (SOCK_DGRAM).
 */
Conn_Status Client::connect() {
	//Try create a socket UDP connection
	_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(_socket < 0){
		perror("Error creating a socket");
		return FAILED;
	}

	// Assign sockadd_in to socket
	memset(&_sin, 0, sizeof(_sin));
	_sin.sin_family = AF_INET;
	_sin.sin_addr.s_addr = inet_addr(_serverIpAddress);
	_sin.sin_port = htons(_serverPortNumber);

	// Return a Conn_Status success if you have reached that point
	return SUCCESS;
}

/*
 * Disconnects client from server.
 * Announce user and return a proper Conn_Status.
 */
Conn_Status Client::disconnect(){
	try{
		close(_socket);
		cout << "Closed socket successfully" << endl;
		return SUCCESS;
	}catch(...){
		perror("Error closing the socket");
		return FAILED;
	}
}

/*
 * Sends one message (data[]) to server.
 * Announce user via appropriate Conn_Status
 */
Conn_Status Client::send(const char data[], int data_len){
	int sent_bytes = sendto(_socket, data, data_len, 0, (struct sockaddr*)&_sin,sizeof(_sin));
	if(sent_bytes < 0){
		perror("Error writing to socket");
		return FAILED;
	}
	return SUCCESS;
}

/*
 * Receive ONE message from server.
 * This method will be called once to get driver's Taxi.
 * From that point the client will be listening constantly.
 */
Conn_Status Client::receiveTaxi(){
	struct sockaddr_in from;
	unsigned int from_len = sizeof(struct sockaddr_in);
	// buffer to read the data into
	char buffer[4096];
	// Receive data by calling recvfrom
	long bytes =	recvfrom(_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);


	if (bytes < 0) {
		// In case data wasn't received properly
		perror("Error reading from socket");
		return FAILED;
	}else{
		try{
			// This method expects only taxi to be received - deserialize.
			Taxi* taxi = new Taxi();
			{
				stringstream taxiStream;
				taxiStream << buffer;
				boost::archive::text_iarchive taxiInArchive(taxiStream);
				taxiInArchive >> *taxi;
			}

			// Assign deserialized taxi to client's driver.
			_driver->assignTaxi(taxi);
			// Avoid dangling pointer
			taxi = NULL;
			return SUCCESS;
		}catch(...){
			//In case data wasn't a taxi as expected
			return FAILED;
		}
	}
}

/*
 * Set Client's driver member
 */
void Client::setDriver(Driver* driver){
	_driver = driver;
}

/*
 * Starts the constant looping of receiving data from server
 */
Conn_Status Client::startReceiving(){
	struct sockaddr_in from;
	unsigned int from_len = sizeof(struct sockaddr_in);
	char buffer[4096];
	long bytes;

	// Change isAlive once you get '7' (server exit code)
	bool isAlive = true;

	while(isAlive){
		bytes  = recvfrom(_socket, buffer, sizeof(buffer), 0, (struct sockaddr *) &from, &from_len);

		// Only when you get an actual data try to parse and work with it
		if(bytes > 0){
			// Save server's message into an input string
			stringstream pointStream;
			pointStream << buffer;
			const string input = pointStream.str();

			// 7 is server's exit code
			if(input == "7"){
				isAlive = false;
				continue;
			}

			// Other than 7, try to get a new location for driver
			try{

				/*
				 * Create a new instance of updateLocation
				 * Deserialize it and update driver's location
				 */
				Point updateLocation;
				{
					boost::archive::text_iarchive pointInArchive(pointStream);
					pointInArchive >> updateLocation;
				}

				// Notify user when you move
				cout << "I have moved to: " << updateLocation << endl;
				_driver->updateLocation(updateLocation);
			}catch(...){
				cout << "Couldn't get new location from server, waiting for next input" << endl;
			}
		}
	} // End of while loop

	return SUCCESS;
}

/*
 * Client's destructor
 * Avoid dangling pointer
 */
Client::~Client() {
	_driver = NULL;
}
