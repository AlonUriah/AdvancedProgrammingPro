/*
 * This is the Server main function.
 * It handles the server creation,
 * and its functionalities.
 */
#include "TaxiCenter.h"
#include <iostream>
#include <stack>
#include <strings.h>

using namespace std;

int main(int argc, char** argv){
	// Try to get a port by arguments
	stringstream ss;
	int port;

	ss << argv[1];
	ss >> port;

	try
	{
		TaxiCenter taxiCenter(port);
	}
	catch (...)
	{
		exit(1);
	}
	return 0;
}
