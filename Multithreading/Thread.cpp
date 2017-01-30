//============================================================================
// Name        : Thread.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : Thread wraps pthread_t with basic thread functionalities
//				 Main thread loop is in the function run
//============================================================================

#include "Thread.h"
#include "../SearchAlgo/Searchable.h"
#include <queue>
#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <stack>
#include "ITask.h"

using namespace std;

// Static members initialization
bool Thread::_shouldRun = false;
queue<ITask*>* Thread::_tasksQueue;
pthread_mutex_t* Thread::_tasksQueueLocker;

/*
 * Constructor
 */
Thread::Thread(){
	_threadId = -1;
	_shouldRun = false;
	_isTasking = false;
}

/*
 * Constructor
 */
Thread::Thread(int threadId, queue<ITask*>* tasksQueue, pthread_mutex_t* tasksQueueLocker){
	_tasksQueue = tasksQueue;
	_tasksQueueLocker = tasksQueueLocker;

	_threadId = threadId;
	_shouldRun = false;
	_isTasking = false;
}

/*
 * Creates and return this wrapper pthread_t
 */
pthread_t* Thread::create(){
	_shouldRun = true;

	pthread_create(&_threadId, NULL, run, this);

	return &_threadId;
}

/*
 * Destructor
 */
Thread::~Thread() {
	_tasksQueue = 0;
	_tasksQueueLocker = 0;
}

/*
 * Stops this thread running by changing
 * its _shouldRun member
 */
void Thread::stop(){
	_shouldRun = false;
}

/*
 * Gets this thread pthread
 */
pthread_t* Thread::getPThread(){
	return &_threadId;
}

/*
 * Returns this Thread _shouldRun member
 */
bool Thread::shouldRun(){
	return _shouldRun;
}

/*
 * Gets the task queue this Thread is listening to
 */
queue<ITask*>* Thread::getTasksQueue(){
	return _tasksQueue;
}

/*
 * Gets the task queue locker
 */
pthread_mutex_t* Thread::getTasksQueueLocker(){
	return _tasksQueueLocker;
}

/*
 * This is the static function that each thread executes
 * by looking at tasks queue, pulling tasks and handle them
 */
void* Thread::run(void* args){
	ITask* taskToHandle = 0;

	Thread* thisThread = (Thread*)args;

	cout << "thread {" << *thisThread->getPThread() << "} is running..." << endl;

	queue<ITask*>* tasksQueue = thisThread->getTasksQueue();

	while(thisThread->shouldRun()){
		if(!tasksQueue->empty()){
			// Pull first task out of tasks queue
			pthread_mutex_t* locker = _tasksQueueLocker;

			pthread_mutex_lock(locker);
				taskToHandle = tasksQueue->front();
				tasksQueue->pop();
			pthread_mutex_unlock(locker);

			cout << "thread {" << *thisThread->getPThread()
					<< "} is taking care of task {" << taskToHandle->getTaskId() <<"}" << endl;

			/*
			 * Task holds everything it needs.
			 * Next iteration of this thread
			 * will only happen after this task ends.
			 */

			if(taskToHandle != 0){
				thisThread->setIsTasking(true);
				taskToHandle->start();
				thisThread->setIsTasking(false);
				delete taskToHandle;
			}
		}
	}

	taskToHandle = 0;
	cout << "thread {" << *thisThread->getPThread() <<"} has finished running" << endl << endl;
	return NULL;
}

/*
 * Sets _isTasking member to a given boolean value
 */
void Thread::setIsTasking(bool isTasking){
	_isTasking = isTasking;
}

/*
 * Returns if this thread is tasking
 */
bool Thread::isTasking(){
	return _isTasking;
}
