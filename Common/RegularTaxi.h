/*
 * This is the RegularTaxi class.
 * It derives the Taxi class, and handles
 * a RegularTaxi type taxi.
 */

#include "Taxi.h"

#include "boost/archive/text_oarchive.hpp"
#include "boost/archive/text_iarchive.hpp"

#include "Taxi.h"

#ifndef REGULARTAXI_H_
#define REGULARTAXI_H_

class RegularTaxi : public Taxi {

public:
	RegularTaxi(){
		id = -1;
		manu = HONDA;
		color = BLUE;
	}

	/*
	 * RegularTaxi constructor
	 */
	RegularTaxi(int id, char manu, char color);
	/*
	 * RegularTaxi destructor
	 */
	virtual ~RegularTaxi();

	/*
	 * Gets the id
	 */
	int getId();
};

#endif /* REGULARTAXI_H_ */
