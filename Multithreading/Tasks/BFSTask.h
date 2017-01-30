//============================================================================
// Name        : BFSTask.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Calculate a route from startPoint to endPoint
//				 and assign it to route.
//				 Changes the trip's status from CALCULATING to PENDING
//============================================================================

/*
 * References for BFS components
 */
#include "../../SearchAlgo/BFS.h"
#include "../../Common/Grid.h"
#include "../../Common/Clock.h"
#include "../../Common/Trip.h"
#include "../IScheduledTask.h"

#include <stack>
#include <pthread.h>

using namespace std;

#ifndef MULTITHREADING_TASKS_BFSTASK_H_
#define MULTITHREADING_TASKS_BFSTASK_H_


class BFSTask : public IScheduledTask {
private:
	Searchable* _start;
	Searchable* _dest;
	Grid* _map;
	Trip* _trip;
	pthread_mutex_t* _mapLocker;

public:
	/*
	 * Constructor
	 */
	BFSTask(int taskId, Grid* map, pthread_mutex_t& mapLocker, Trip* trip, Searchable* startPoint,
			Searchable* destPoint);

	/*
	 * This Task locks the map,
	 * calculating route, assign it to trip and change its status.
	 * Notify user.
	 */
	virtual void start();

	/*
	 * Destructor
	 */
	virtual ~BFSTask();

};

#endif /* MULTITHREADING_TASKS_BFSTASK_H_ */
