//============================================================================
// Name        : Node.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Simplest implementation of a grid node.
//				 This could be later on have more features.
//				 It is based on searchable.
//============================================================================

#include "Node.h"
using namespace std;

/*
 * This is the default constructor.
 * At the moment it does nothing but call searchable's
 * base constructor. This is done because we expect
 * the later on node will have more features.
 */
Node::Node() : Searchable() { }

/*
 * Calls searchable's base constructor using
 * a given point.
 */
Node::Node(Point& value) : Searchable(value){ }

/*
 * Copy constructor.
 * Based on searchable second constructor (Point given).
 */
Node::Node(Node*& other) : Searchable(other->getValue()){ }

/*
 * Sets this node value with a given point
 */
void Node::setValue(Point& point){
	_value = point;
}

/*
 * Write this node by printing its _value Point
 */
ostream& operator << (ostream& os, const Node &node){
	return os << node._value;
}

/*
 * Assignment operator overloading
 */
Node& Node::operator = (const Node &n1){
	if(this == &n1){
		return *this;
	}
	_value = n1._value;
	_distance = n1._distance;
	_parent = n1._parent;
	_isAvalable = n1._isAvalable;
	return *this;
}

/*
 * Default destructor.
 */
Node::~Node() {
}

