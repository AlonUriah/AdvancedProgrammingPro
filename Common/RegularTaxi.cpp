/*
 * This is the RegularTaxi class.
 * It derives the Taxi class, and handles
 * a RegularTaxi type taxi.
 */
#include "Taxi.h"

#include "boost/serialization/export.hpp"
#include "RegularTaxi.h"

/*
 * RegularTaxi constructor
 */
RegularTaxi::RegularTaxi(int id, char manu, char color) : Taxi(id, manu, color) { }
/*
 * RegularTaxi destructor
 */
RegularTaxi::~RegularTaxi() { }
/*
 * Gets the id
 */
int RegularTaxi::getId()
{
	return this->id;
}
