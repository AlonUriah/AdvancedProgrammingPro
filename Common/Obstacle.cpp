//============================================================================
// Name        : Obstacle.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Obstacle is basically a node with the value of _isAvailable
//				 set to false.
//				 This obstacle may also have a description for later
//				 features such as 'look around' in map, etc.
//============================================================================

#include "Obstacle.h"
#include "Node.h"
#include "Point.h"

/*
 * Constructs a new empty Obstacle.
 */
Obstacle::Obstacle() : Node(){
	_description = 0;
}

/*
 * Constucts a new Obstacle at a given location
 */
Obstacle::Obstacle(Point location) : Node(location){
	_isAvalable=false;
	_description = 0;
}

/*
 * Constructs a new Obstacle.
 */
Obstacle::Obstacle(Point* location, char* desc) : Node(*location)
{
	_description = desc;
}
/*
 * Destructs the current Obstacle.
 */
Obstacle::~Obstacle() {

}
/*
 * Gets the description of the obstacle.
 */
char* Obstacle::getDescription()
{
	return _description;
}
