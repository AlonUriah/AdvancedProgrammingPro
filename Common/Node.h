//============================================================================
// Name        : Node.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Simplest implementation of a grid node.
//				 This could be later on have more features.
//				 It is based on searchable.
//============================================================================
#include "Point.h"
#include "../SearchAlgo/Searchable.h"

#ifndef HEADERS_NODE_H_
#define HEADERS_NODE_H_

using namespace std;

class Node : public Searchable{
public:
	/*
	 * This is the default constructor.
	 * At the moment it does nothing but call searchable's
	 * base constructor. This is done because we expect
	 * the later on node will have more features.
	 */
	Node();

	/*
	 * Calls searchable's base constructor using
	 * a given point.
	 */
	Node(Point& value);

	/*
	 * Copy constructor.
	 * Based on searchable second constructor (Point given).
	 */
	Node(Node*& other);

	/*
	 * Sets this node value with a given point
	 */
	void setValue(Point& point);

	/*
	 * Write this node by printing its _value Point
	 */
	friend ostream& operator << (ostream& os, const Node &node);

	/*
	 * Assignment operator overloading
	 */
	Node& operator = (const Node &n1);

	/*
	 * Default destructor.
	 */
	~Node();
};

#endif /* HEADERS_NODE_H_ */
