/*
 * This is the Server class.
 * It handles the whole server
 * functionalities, including the input
 * from the server manager, and send/receive,
 * inputs from the clients.
 * Moreover, it handles the TaxiCenter, which
 * manages the whole data.
 */
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
#include "TaxiCenter.h"
#include "../Common/Grid.h"
#include "../Common/Common.h"
#include "../Common/Factories/GridFactory.h"
#include "../SearchAlgo/BFS.h"
#include <list>
#include <stdexcept>

using namespace std;

#ifndef SERVER_H_
#define SERVER_H_
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
	void Start();
	/*
	 * The method handles the input from the user.
	 * for each number, there is a specific action.
	 */
	void input();
	/*
	 * The method gets a Point, and send
	 * it to the client. This is for trailing
	 * its route.
	 */
	void sendTarget(Point& p);
	/*
	 * The method aborts the connection.
	 * It happens while sending the character
	 * '7' as an input.
	 */
	void Abort();
private:
	int port;
	struct sockaddr_in server_addr;
	int serverSocket;
	struct sockaddr_in client;
	unsigned int client_len;
	int cliendId;
	TaxiCenter* taxiCenter;
};

#endif /* SERVER_H_ */
