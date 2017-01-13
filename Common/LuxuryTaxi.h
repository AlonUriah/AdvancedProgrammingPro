/*
 * This is the LuxuryTaxi class.
 * It derives the Taxi class, and handles
 * a LuxuryTaxi type taxi.
 */


#ifndef LUXURYTAXI_H_
#define LUXURYTAXI_H_
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include "Taxi.h"


class LuxuryTaxi : public Taxi {
public:
	LuxuryTaxi();
	/*
	 * LuxuryTaxi constructor
	 */
	LuxuryTaxi(int id, char manu, char color);
	/*
	 * LuxuryTaxi destructor
	 */
	virtual ~LuxuryTaxi();
};
#endif /* LUXURYTAXI_H_ */
