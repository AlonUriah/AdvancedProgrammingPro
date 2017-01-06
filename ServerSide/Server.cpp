/*
 * This is the Server class.
 * It handles the whole server
 * functionalities, including the input
 * from the server manager, and send/receive,
 * inputs from the clients.
 * Moreover, it handles the TaxiCenter, which
 * manages the whole data.
 */
#include "Server.h"
/*
 * A server constructor.
 */
Server::Server(int port) {
	/*
	 * Initialize varialbes
	 */
	this->cliendId = -1;
	this->port = port;
	this->serverSocket = -1;
	this->taxiCenter = new TaxiCenter();
	this->client_len = sizeof(struct sockaddr_in);
	/*
	 * Start reading input from the user.
	 */
	this->input();

}
/*
 * Server destructor
 */
Server::~Server() {
	delete this->taxiCenter;
}
/*
 * The method start a listening connection.
 * In the future, it will listen as a different
 * Thread, while each incoming connection will be
 * followed to another Thread.
 */
void Server::Start()
{
	try
	{
		// Open a listening socket, UDP type.
		this->serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
		if (this->serverSocket == -1)
			perror("Error: socket was not able to initiate.");

		// Define the server details.
		memset(&this->server_addr, 0, sizeof(this->server_addr));
		this->server_addr.sin_family = AF_INET;
		this->server_addr.sin_addr.s_addr = INADDR_ANY;
		this->server_addr.sin_port = htons(this->port);
		bzero(&(this->server_addr.sin_zero), 8);

		// Try to bind the port.
		if (bind(this->serverSocket, (struct sockaddr*)&this->server_addr,
				sizeof(struct sockaddr)) == -1)
		{
			perror("Error: bind was not able to initiate.");
			return;
		}
		/*
		 * Handling clients
		 */
		Taxi* taxi = NULL;
		try
		{

			// Receive a driver object from the client.
			char buffer[4096];
			long bytes = recvfrom(this->serverSocket, buffer, sizeof(buffer), 0,
					(struct sockaddr *) &this->client, &this->client_len);
			// If receive was successful...
			if (bytes > 0) {
				// Deserialize the driver.
				stringstream bufferStream;
				bufferStream << buffer;
				Driver* driver = new Driver();
				{
					boost::archive::text_iarchive ia(bufferStream);
					ia >> *driver;
				}
				// Store the driver's id.
				this->cliendId = driver->getId();
				// Add the driver to the drivers' list, and pull a Taxi from the list.
				taxi = this->taxiCenter->addDriver(driver);
				// Zero to pointer.
				driver = NULL;
			}
		}
		catch (...)
		{
			// Close the socket if there's an error.
			close(this->serverSocket);
			return;
		}
		// If the taxi is not null, means the driver has a taxi.
		if (taxi != NULL)
		{
			// Serialize the taxi.
			stringstream taxiStream;
			boost::archive::text_oarchive oa(taxiStream);
			oa << *taxi;

			const string tmp = taxiStream.str();
			const char* data = tmp.c_str();
			int data_len = strlen(data);
			// Send the taxi to the client.
			try
			{
				int sent_bytes = sendto(this->serverSocket, data, data_len, 0,
						(struct sockaddr *) &this->client, this->client_len);
				if (sent_bytes < 0) {
					perror("Error: couldn't send taxi.");
				}
				// Zero to pointer.
				taxi = NULL;
			}
			catch (...)
			{
				// Close the socket.
				close(this->serverSocket);
				taxi = NULL;
			}
		}
	}
	/*
	 * If the connection has lost, we can
	 * close it, and return.
	 */
	catch (...)
	{
		close(this->serverSocket);
		return;
	}
	return;
}
/*
 * The method gets a Point, and send
 * it to the client. This is for trailing
 * its route.
 */
void Server::sendTarget(Point& p)
{
	// Serialize the point.
	Point p1(p);
	stringstream pointStream;
	boost::archive::text_oarchive oa(pointStream);
	oa << p1;

	const string tmp = pointStream.str();
	const char* data = tmp.c_str();
	int data_len = strlen(data);
	// Send the point to the client.
	try
	{
		int sent_bytes = sendto(this->serverSocket, data, data_len, 0, (struct sockaddr *) &this->client, this->client_len);
		if (sent_bytes < 0) {
			perror("Error: couldn't send point.");
		}
	}
	catch (...)
	{
		cout << "Error: couldn't send point." << endl;
	}
}
/*
 * The method aborts the connection.
 * It happens while sending the character
 * '7' as an input.
 */
void Server::Abort()
{
	// Send the client '7' to notify the server is going to close.
	try
	{
		int sent_bytes = sendto(this->serverSocket, "7", 1, 0, (struct sockaddr *) &this->client, this->client_len);
		if (sent_bytes < 0) {
			perror("Error: couldn't send exit sign.");
		}
	}
	catch (...)
	{
		cout << "Error: couldn't send exit sign." << endl;
	}
	// Close the server.
	close(this->serverSocket);
}
/*
 * The method handles the input from the user.
 * for each number, there is a specific action.
 */
void Server::input()
{
	// Declarations.
	TripWrapper* tripWrapper;
	TaxiWrapper* taxiWrapper;
	int id;
	int numOfDrivers;
	int userChoice;
	string input;
	// Input loop.
	do
	{
		// Input a number.
		cin >> userChoice;

		switch(userChoice){
		// Input drivers
			case 1:
				cin >> numOfDrivers;
				this->Start();
				break;
		// Input a trip.
			case 2:
				tripWrapper = parseTrip();
				this->taxiCenter->addRide(tripWrapper->id,tripWrapper->startX,
										  tripWrapper->startY, tripWrapper->endX,
										  tripWrapper->endY,tripWrapper->passengers,
										  tripWrapper->tariff, tripWrapper->startTime);
				delete tripWrapper;
				break;
		// Input a taxi.
			case 3:
				taxiWrapper = parseTaxi();
				this->taxiCenter->addTaxi(taxiWrapper->id,taxiWrapper->type,
									      taxiWrapper->manu,taxiWrapper->color);
				delete taxiWrapper;
				break;
		// Retrieve a location of a driver.
			case 4:
				cin >> id;
				cout << this->taxiCenter->getDriversLocation(id) << endl;
				break;
		// Abort the program.
			case 7:
				this->Abort();
				break;
		// Move the time forward by 1.
			case 9:
				this->taxiCenter->advanceTime();
				this->sendTarget(this->taxiCenter->getDriversLocation(this->cliendId));
				break;
			default:
				break;
		}
	} while (userChoice != 7);
}