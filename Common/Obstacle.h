//============================================================================
// Name        : Obstacle.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Obstacle is basically a node with the value of _isAvailable
//				 set to false.
//				 This obstacle may also have a description for later
//				 features such as 'look around' in map, etc.
//============================================================================

#ifndef OBSTACLE_H_
#define OBSTACLE_H_

#include "Node.h"
#include "Point.h"
#include <iostream>

using namespace std;

class Obstacle : public Node{
private:
	char* _description;
public:
	/*
	 * Constructs a new empty Obstacle.
	 */
	Obstacle();
	/*
	 * Constucts a new Obstacle at a given location
	 */
	Obstacle(Point location);
	/*
	 * Constructs a new Obstacle.
	 */
	Obstacle(Point* location, char* desc);
	/*
	 * Destructs the current Obstacle.
	 */
	virtual ~Obstacle();
	/*
	 * Gets the description of the obstacle.
	 */
	char* getDescription();
};

#endif /* OBSTACLE_H_ */
