/*
 * ClientData.h
 *
 *  Created on: Jan 28, 2017
 *      Author: alon
 */
//#include "../ServerSide/Server.h"

#ifndef CLIENTDATA_H_
#define CLIENTDATA_H_

class ClientData {
public:
	int client;
	int clientVal;
	unsigned int clientLen;
	int driverId;
	//Server* server;

	ClientData();
	ClientData& operator = (const ClientData& other);
	bool operator == (const ClientData& other) const;
	virtual ~ClientData();
};

#endif /* CLIENTDATA_H_ */
