/*
 * This is the Taxi class.
 * This is an abstract class, which
 * is the father of two other classes -
 * RegularTaxi and LuxuryTaxi.
 */
#include <iostream>
#include "Common.h"

using namespace std;

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#ifndef TAXI_H_
#define TAXI_H_
class Taxi {
public:
	Taxi(){
		id = -1;
		manu = HONDA;
		color = BLUE;
		speed = 1;
	};
	/*
	 * Construct a new taxi.
	 */
	Taxi(int id, char manu, char color);
	/*
	 * Destructs a taxi.
	 */
	virtual ~Taxi();
	/*
	 * Returns the taxi's id.
	 */
	virtual int getId();
	/*
	 * Returns the speed.
	 */
	int getSpeed();
	// gTest purposes
	int getKm();
	void setKm(int km);
protected:
	int id;
	Manufacturer manu;
	Color color;
	int speed;


private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version){
		archive & BOOST_SERIALIZATION_NVP(id);
		archive & BOOST_SERIALIZATION_NVP(manu);
		archive & BOOST_SERIALIZATION_NVP(color);
	};
};

#endif /* TAXI_H_ */
