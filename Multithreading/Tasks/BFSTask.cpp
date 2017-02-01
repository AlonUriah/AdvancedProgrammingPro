//============================================================================
// Name        : BFSTask.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Calculate a route from startPoint to endPoint
//				 and assign it to route.
//				 Changes the trip's status from CALCULATING to PENDING
//============================================================================

/*
 * References for BFS components
 */
#include "BFSTask.h"
#include "../../Common/Common.h"
#include "../../Common/Grid.h"
#include "../../Common/Trip.h"
#include "../../SearchAlgo/Searchable.h"
#include "../../SearchAlgo/BFS.h"
#include "../IScheduledTask.h"
#include "../ITaskHandler.h"

#include <stack>
#include <pthread.h>

using namespace std;

/*
 * Constructor
 */
BFSTask::BFSTask(int taskId, Grid* map, pthread_mutex_t& mapLocker,
				Trip* trip, Searchable* startPoint, Searchable* destPoint)
				: IScheduledTask(taskId)
{
	_start = startPoint;
	_dest = destPoint;
	_map = map;
	_mapLocker = &mapLocker;
	_trip = trip;
}

/*
 * This Task locks the map,
 * calculating route, assign it to trip and change its status.
 * Notify user.
 */
void BFSTask::start(){
	pthread_mutex_lock(_mapLocker);
	stack<Searchable*>* root = BFS::getRoot(*_map,_start,_dest);
	pthread_mutex_unlock(_mapLocker);

	// Pop starting point
	root->pop();

	_trip->assignRoute(root);
	_trip->setStatus(PENDING);
	
	root = NULL;
	stringstream ss;
	ss << "Done calculating route to trip id " << _trip->getId();
	_logger->info(ss.str());
}

/*
 * Destructor
 */
BFSTask::~BFSTask() {
	_clock = 0;
	_dest = 0;
	_map = 0;
	_mapLocker = 0;
	_start = 0;
	_taskHandler = 0;
}
