//============================================================================
// Name        : Point.h
// Author      : Alon Aviv Uriah Ahrak
// Description : A base class for Point.
// 				 Holds a X and Y integers.
//============================================================================
#ifndef HEADERS_POINT_H_
#define HEADERS_POINT_H_

// Members
#include <iostream>

// Serialization Members
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;
class Point {
private:
	int _x;
	int _y;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& archive, const unsigned int version){
		archive & BOOST_SERIALIZATION_NVP(_x);
		archive & BOOST_SERIALIZATION_NVP(_y);
	}

public:
	/*
	 * Default constructor
	 */
	Point();

	/*
	 * Creates a point with given x and y values
	 */
	Point(int x, int y);

	/*
	 * Copy constructor
	 */
	Point(Point& other);

	/*
	 * Sets this point x value
	 */
	void setX(int x);

	/*
	 * Gets this x value
	 */
	int getX();

	/*
	 * Sets  this point y value
	 */
	void setY(int y);

	/*
	 * Gets this y value
	 */
	int getY();

	/*
	 * Write this point in the following format (x,y)
	 */
	friend ostream& operator << (ostream& os, const Point &point);

	/*
	 * Two points are equal iff their values equal
	 */
	bool operator == (const Point &p1)const;

	/*
	 * Assignment operator overloading
	 */
	Point& operator = (const Point &p1);

	/*
	 * Destrtuctor
	 */
	~Point();
};

#endif /* HEADERS_POINT_H_ */
