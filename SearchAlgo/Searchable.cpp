//============================================================================
// Name        : Searchable.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : A base class that supports BFS methods
//============================================================================

#include "Searchable.h"
using namespace std;

/*
 * Default constructor
 */
Searchable::Searchable() {
	_value = Point(0,0);
	_distance = -1;
	_parent = 0;
	_isAvalable = true;
}

/*
 * Sets the value to a given point
 */
Searchable::Searchable(Point& value){
	_value = value;
	_distance = -1;
	_parent = 0;
	_isAvalable = true;
}

/*
 * Gets the value of this node
 */
Point& Searchable::getValue(){
	return _value;
}

/*
 * Sets the distance to a given one
 */
void Searchable::setDistance(int distance){
	_distance = distance;
}

/*
 * Gets the distance
 */
int Searchable::getDistance(){
	return _distance;
}

/*
 * Setting _parent to point to another searchable
 */
void Searchable::setParent(Searchable*& parent){
	if(this == parent){
		throw invalid_argument("Cannot assign a node to itself");
	}
	_parent = parent;
}

/*
 * Returns _parent
 */
Searchable* Searchable::getParent(){
	return _parent;
}

/*
 * Assignment operator overloading
 */
Searchable& Searchable::operator = (const Searchable& s1){
	if(this == &s1){
		return *this;
	}
	_parent = s1._parent;
	_distance = s1._distance;
	_value = s1._value;
	_isAvalable = s1._isAvalable;
	return *this;
}

/*
 * Two searchables are equal iff their value is the same
 */
bool Searchable::operator == (const Searchable& s1)const{
	if(_value == s1._value){
		return true;
	}
	return false;
}

/*
 * Writing this searchable value
 */
ostream& operator << (ostream& os, const Searchable& s1){
	return os << s1._value;
}

/*
 * Return if this searchable is available.
 * Will be used later one to identify non-through
 * searchable nodes.
 */
bool Searchable::isAvailable(){
	return _isAvalable;
}

/*
 * Delete this searchable
 */
Searchable::~Searchable() { }

/*
 * setAvailability changes this Node's state by changing
 * its _isAvailable member to a given isAvailable argument.
 * Relevant if we will take into consideration intersections.
 * For example - if car A is at position B so B should NOT be
 * available for any car C.
 */
void Searchable::setAvailability(bool isAvailable){
	_isAvalable = isAvailable;
}

