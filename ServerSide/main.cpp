/*
 * This is the Server main function.
 * It handles the server creation,
 * and its functionalities.
 */
#include "TaxiCenter.h"
#include <iostream>
#include <stack>

using namespace std;

int main(int argc, char** argv){
	// Try to get a port by arguments
	try
	{
		TaxiCenter taxiCenter(atoi(argv[1]));
	}
	catch (...)
	{
		exit(1);
	}
	return 0;
}
