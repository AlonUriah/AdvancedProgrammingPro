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
	this->port = port;
	this->serverSocket = -1;
	this->clients = new list<ClientData*>;

	// Initialize mutex
	pthread_mutex_init(&this->clients_locker, 0);

	// Get the logger singleton
	this->logger = Logger::getInstance();
	this->logger->shouldPrint(true);
	this->logger->info("SERVER Initialized.");
}
/*
 * Server destructor
 */
Server::~Server() {
	// Destroy the mutex
	pthread_mutex_destroy(&this->clients_locker);
	// Delete the clients' data
	ClientData* pointer = NULL;;
	while (!this->clients->empty())
	{
		pointer = this->clients->front();
		pointer->server = NULL;
		this->clients->pop_front();
		delete pointer;
	}
	delete this->clients;
	this->logger->info("SERVER Destroyed.");
	delete this->logger;
}
/*
 * The method start a listening connection.
 * In the future, it will listen as a different
 * Thread, while each incoming connection will be
 * followed to another Thread.
 */
void Server::handleClients(int num)
{
	try
	{
		/*
		 * If the server wasn't created,
		 * create it.
		 * else, it is already listening for clients,
		 * so keep on getting some clients.
		 */
		if (this->serverSocket < 0)
		{
			this->logger->info("Initializing socket...");

			// Open a listening socket, TCP type.
			this->serverSocket = socket(PF_INET, SOCK_STREAM, 0);
			if (this->serverSocket == -1)
				this->logger->warn("Socket was not able to create.");

			this->logger->info("Setting server details...");

			// Define the server details.
			memset(&this->server_addr, 0, sizeof(this->server_addr));
			this->server_addr.sin_family = PF_INET;
			this->server_addr.sin_addr.s_addr = INADDR_ANY;
			this->server_addr.sin_port = htons(this->port);
			bzero(&(this->server_addr.sin_zero), 8);

			this->logger->info("Binding the port...");

			// Try to bind the port.
			int bindRet = ::bind(this->serverSocket, 
(struct sockaddr*)&this->server_addr,
                                        sizeof(struct sockaddr)); 
			if (bindRet < 0)
			{
				this->logger->warn("Port was not able to be binded.");
				return;
			}

			this->logger->info("Server is now listening.");

			// Start listening to broadband
			listen(this->serverSocket, 5);
		}
		// Create a threads pool of 'num' threads
		pthread_t threadsPool[num];
		/*
		 * Handling clients
		 */
		int i = 0;
		while (i < num)
		{
			try
			{
				// Declare on client's data to preserve
				ClientData* currentClient;
				int client, client_val;
				unsigned int client_len = sizeof(client);

				// Accept a new client
				client_val = accept(this->serverSocket, (struct sockaddr*)&client, &client_len);
				if (client_val < 0)
				{
					this->logger->warn("Client was unable to connect.");
				}
				else
				{
					this->logger->info("Client accepted.");

					// Create a new client data type, and store the data
					currentClient = new ClientData();
					currentClient->client = client;
					currentClient->client_val = client_val;
					currentClient->client_len = client_len;
					currentClient->server = this;

					// Create a special thread for the client, with the callHandleClient function
					pthread_create(&threadsPool[i], NULL, callHandleClient, (void*)(currentClient));
					// Avoid dangling pointer
					currentClient = NULL;
				}
			}
			catch (...)
			{
				this->logger->info("An error occurred. Server is closing...");

				// Close the socket if there's an error.
				close(this->serverSocket);

				this->logger->info("Server is closed.");

				return;
			}
			i++;
		}
		/*
		 * Wait for all the clients to finish the registration
		 * process, of sending a 'Driver' object, and getting
		 * back a 'Taxi' object.
		 */
		for (int i = 0; i < num; i++)
			pthread_join(threadsPool[i], NULL);

		this->logger->info("Finished handling clients.");

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
 * Handle a specific client, in a different thread.
 */
void* Server::handleClient(void* element)
{

	// Declare data pointers
	Taxi* taxi = NULL;
	Driver* driver = NULL;
	// Cast the client data
	ClientData* currentClient = (ClientData*)element;
	// Avoid dangling pointer
	element = NULL;

	this->logger->info("Handling client.");

	// Declare a buffer, and reset it
	char buffer[4096];
	memset(buffer, 0, sizeof(buffer));

	// Get the Driver from the current client
	long bytes = recv(currentClient->client_val, buffer, sizeof(buffer), 0);
	if (bytes > 0)
	{
		// Deserialize the driver.
		stringstream bufferStream;
		bufferStream << buffer;
		driver = new Driver();
		{
			boost::archive::text_iarchive ia(bufferStream);
			ia >> *driver;
		}
		// Set the client id, to be the driver's one
		currentClient->driverId = driver->getId();

		this->logger->info("Driver received.");

		// Store the client data
		pthread_mutex_lock(&this->clients_locker);
		this->clients->push_back(currentClient);
		pthread_mutex_unlock(&this->clients_locker);

		// Add the driver to the drivers' list, and pull a Taxi from the list.
		taxi = this->addDriver(driver);
		// Avoid dangling pointer

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
				int sent_bytes = send(currentClient->client_val, data, data_len, 0);
				if (sent_bytes < 0) {
					this->logger->warn("Taxi cannot be sent.");
				}
				else
				{
					this->logger->info("Taxi was sent.");
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
	// Avoid dangling pointers
	currentClient = NULL;
	driver = NULL;
	return NULL;
}
/*
 * The method gets a Point, and send
 * it to the client. This is for trailing
 * its route.
 */
void Server::broadcast()
{
	this->logger->info("Broadcasting points to clients.");

	/*
	 * Iterate each client data, and send them the
	 * next point they should be at.
	 */
	list<ClientData*>::iterator it = this->clients->begin();
	while (it != this->clients->end())
	{
		// Get the point by the template design pattern
		Point p1(this->getDriversLocation((*it)->driverId));
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
			int sent_bytes = send((*it)->client_val, data, data_len, 0);
			if (sent_bytes < 0) {
				this->logger->warn("Point was not able to be sent.");
			}
			else
			{
				this->logger->info("Point was sent.");
			}
		}
		catch (...)
		{
			this->logger->warn("An error occurred. Point couldn't not be sent.");
		}
		++it;
	}
}
/*
 * The method aborts the connection.
 * It happens while sending the character
 * '7' as an input.
 */
void Server::Abort()
{
	this->logger->info("Server is closing...");

	// Send the client '7' to notify the server is going to close.
	for (list<ClientData*>::iterator it = this->clients->begin(); it != this->clients->end(); ++it)
	{
		try
		{
			int sent_bytes = send((*it)->client_val, "7", 1, 0);
			if (sent_bytes < 0) {
				this->logger->info("Exit sign was not able to be sent.");
			}
		}
		catch (...)
		{
			this->logger->info("Exit sign was not able to be sent.");
		}
	}

	// Close the server.
	close(this->serverSocket);
	this->logger->info("Server is closed.");

}
/*
 * Calling the handleClient method.
 * This is a static method, since the pthread_create
 * function receives a specific pointer to function,
 * and it can't be an inner method class one, because
 * the 'this' reference is hidden among the
 * method parameters.
 */
void* Server::callHandleClient(void* element)
{
	// Cast the client data
	ClientData* currentClient = (ClientData*)element;
	// Call the thread function
	currentClient->server->handleClient(element);
	// Avoid dangling pointers
	currentClient = NULL;
	return NULL;
}
