//============================================================================
// Name        : Point.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : A base class for Point.
// 				 Holds a X and Y integers.
//============================================================================

#include "Point.h"
#include <iostream>

/*
 * Default constructor
 */
Point::Point() {
	_x = 0;
	_y = 0;
}

/*
 * Creates a point with given x and y values
 */
Point::Point(int x, int y){
	_x = x;
	_y = y;
}

/*
 * Copy constructor
 */
Point::Point(Point& other){
	_x = other._x;
	_y = other._y;
}

/*
 * Sets this point x value
 */
void Point::setX(int x){
	_x = x;
}

/*
 * Gets this x value
 */
int Point::getX(){
	return _x;
}

/*
 * Sets  this point y value
 */
void Point::setY(int y){
	_y = y;
}

/*
 * Gets this y value
 */
int Point::getY(){
	return _y;
}

/*
 * Write this point in the following format (x,y)
 */
ostream& operator << (ostream& os, const Point &point){
	return os << "(" << point._x << "," << point._y << ")";
}

/*
 * Two points are equal iff their values equal
 */
bool Point::operator == (const Point &p1)const{
	return (_x == p1._x && _y == p1._y);
}

/*
 * Assignment operator overloading
 */
Point& Point::operator = (const Point& p1){
	if(this == &p1){
		return *this;
	}
	_x = p1._x;
	_y = p1._y;
	return *this;
}

/*
 * Destrtuctor
 */
Point::~Point() {
}

