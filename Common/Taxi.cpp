/*
 * This is the Taxi class.
 * This is an abstract class, which
 * is the father of two other classes -
 * RegularTaxi and LuxuryTaxi.
 */
#include "Taxi.h"

/*
 * Construct a new taxi.
 */
Taxi::Taxi(int id, char manu, char color)
{
	this->speed = 1;
	this->id = id;

	switch (manu)
	{
		case 's':
		case 'S':
			this->manu = SUBARU;
			break;
		case 'h':
		case 'H':
			this->manu = HONDA;
			break;
		case 't':
		case 'T':
			this->manu = TESLA;
			break;
		case 'f':
		case 'F':
			this->manu = FIAT;
			break;
	}
	switch (color)
	{
		case 'r':
		case 'R':
			this->color = RED;
			break;
		case 'g':
		case 'G':
			this->color = GREEN;
			break;
		case 'b':
		case 'B':
			this->color = BLUE;
			break;
		case 'w':
		case 'W':
			this->color = WHITE;
			break;
		case 'p':
		case 'P':
			this->color = PINK;
			break;
	}
}
/*
 * Returns the id.
 */
int Taxi::getId(){
	return this->id;
}
/*
 * Returns the speed.
 */
int Taxi::getSpeed()
{
	return this->speed;
}
/*
 * Taxi destructor.
 */
Taxi::~Taxi() { }
