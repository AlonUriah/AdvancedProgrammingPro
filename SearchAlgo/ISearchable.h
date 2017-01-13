/*
 * ISearchable.h
 *
 *  Created on: Jan 13, 2017
 *      Author: alon
 */
#include "../Common/Point.h"
#include <vector>

#ifndef SEARCHALGO_ISEARCHABLE_H_
#define SEARCHALGO_ISEARCHABLE_H_

class ISearchable {
protected:
	bool _isAvailable;
	int _distance;

	Point _value;
	Searchable* _parent;
	vector<Searchable*> _neighbors;

public:
	void addNeighbor(Searchable* neighbor);
	vector<Searchable*> getNeighbors;
};



#endif /* SEARCHALGO_ISEARCHABLE_H_ */
