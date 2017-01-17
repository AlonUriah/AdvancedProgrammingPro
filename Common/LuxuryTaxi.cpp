/*
 * This is the LuxuryTaxi class.
 * It derives the Taxi class, and handles
 * a LuxuryTaxi type taxi.
 */
#include "Taxi.h"

#include <boost/serialization/export.hpp>
#include "LuxuryTaxi.h"

LuxuryTaxi::LuxuryTaxi() : Taxi(){
	id = -1;
	manu = HONDA;
	color = BLUE;
}

/*
 * LuxuryTaxi constructor
 */
LuxuryTaxi::LuxuryTaxi(int id, char manu, char color) : Taxi(id, manu, color) {
	this->speed = 2;
}
/*
 * LuxuryTaxi destructor
 */
LuxuryTaxi::~LuxuryTaxi() { }
/*
 * Gets the id
 */
int LuxuryTaxi::getId()
{
	return this->id;
}