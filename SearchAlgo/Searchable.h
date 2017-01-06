//============================================================================
// Name        : Searchable.h
// Author      : Alon Aviv Uriah Ahrak
// Description : A base class that supports BFS methods
//============================================================================
#include "../Common/Point.h"
#include <stdexcept>
#include <vector>

using namespace std;

#ifndef HEADERS_SEARCHABLE_H_
#define HEADERS_SEARCHABLE_H_

class Searchable {
protected:
	//Place in grid
	Point _value;
	//Relatively to another searchable
	int _distance;
	//To implement the BFS algorithm
	Searchable* _parent;
	//To later deal with obstacles etc
	bool _isAvalable;

public:
	/*
	 * Default constructor
	 */
	Searchable();

	/*
	 * Sets the value to a given point
	 */
	Searchable(Point& value);

	/*
	 * Gets the value of this node
	 */
	Point& getValue();

	/*
	 * Sets the distance to a given one
	 */
	void setDistance(int distance);

	/*
	 * Gets the distance
	 */
	int getDistance();

	/*
	 * Setting _parent to point to another searchable
	 */
	virtual void setParent(Searchable*& parent);

	/*
	 * Returns _parent
	 */
	virtual Searchable* getParent();

	/*
	 * Assignment operator overloading
	 */
	Searchable& operator = (const Searchable &s1);

	/*
	 * Two searchables are equal iff their value is the same
	 */
	bool operator == (const Searchable& s1)const;

	/*
	 * Writing this searchable value
	 */
	friend ostream& operator << (ostream& os, const Searchable& s1);

	/*
	 * Return if this searchable is available.
	 * Will be used later one to identify non-through
	 * searchable nodes.
	 */
	bool isAvailable();

	/*
	 * Delete this Searchable
	 */
	virtual ~Searchable();

	/*
	 * setAvailability changes this Node's state by changing
	 * its _isAvailable member to a given isAvailable argument.
	 * Relevant if we will take into consideration intersections.
	 * For example - if car A is at position B so B should NOT be
	 * available for any car C.
	 */
	void setAvailability(bool isAvailable);
};

#endif /* HEADERS_SEARCHABLE_H_ */
