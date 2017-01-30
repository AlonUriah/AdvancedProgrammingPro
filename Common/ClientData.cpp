/*
 * ClientData.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: alon
 */

#include "ClientData.h"
//class Server;

ClientData::ClientData() {
	client = -1;
	clientVal = -1;
	clientLen = 0;
	driverId = -1;
	//server = 0;
}

ClientData& ClientData::operator =(const ClientData& other){
	if(*this == other){
		return *this;
	}

	client = other.client;
	clientVal = other.clientVal;
	clientLen = other.clientLen;
	driverId = other.driverId;
	//server = other.server;
	return *this;
}

bool ClientData::operator == (const ClientData& other) const{
	if(client != other.client
		|| clientVal != other.clientVal
		|| clientLen != other.clientLen
		|| driverId != other.driverId){
		return false;
	}

	return true;
}

ClientData::~ClientData() {
	//server = 0;
}

