//============================================================================
// Name        : Thread.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Thread wraps pthread_t with basic thread functionalities
//				 Main thread loop is in the function run
//============================================================================

#include <pthread.h>
#include <queue>
#include "ITask.h"

#ifndef MULTITHREADING_THREAD_H_
#define MULTITHREADING_THREAD_H_

using namespace std;

class Thread {
private:
	pthread_t _threadId;
	bool _isTasking;

	static bool _shouldRun;
	static queue<ITask*>* _tasksQueue;
	static pthread_mutex_t* _tasksQueueLocker;

public:
	/*
	 * Constructor
	 */
	Thread();

	/*
	 * Constructor
	 */
	Thread(int threadId, queue<ITask*>* tasksQueue, pthread_mutex_t* tasksQueueLocker);

	/*
	 * Creates and return this wrapper pthread_t
	 */
	pthread_t* create();

	/*
	 * This is the static function that each thread executes
	 */
	static void* run(void* args);

	/*
	 * Stops this thread running by changing
	 * its _shouldRun member
	 */
	void stop();

	/*
	 * Returns this Thread _shouldRun member
	 */
	bool shouldRun();

	/*
	 * Gets the task queue this Thread is listening to
	 */
	queue<ITask*>* getTasksQueue();

	/*
	 * Gets the task queue locker
	 */
	pthread_mutex_t* getTasksQueueLocker();

	/*
	 * Gets this thread pthread
	 */
	pthread_t* getPThread();

	/*
	 * Sets _isTasking member to a given boolean value
	 */
	void setIsTasking(bool isTasking);

	/*
	 * Returns if this thread is tasking
	 */
	bool isTasking();

	/*
	 * Destructor
	 */
	virtual ~Thread();
};

#endif /* MULTITHREADING_THREAD_H_ */
