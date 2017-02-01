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

struct threadData{
	int _threadId;
	bool* _shouldRun;
	bool* _isTasking;
	queue<ITask*>* _tasksQueue;
	pthread_mutex_t* _tasksQueueLocker;
};

/*
 * Constructor
 */
Thread::Thread(){
	_threadId = -1;
	_shouldRun = false;
	_isTasking = false;
	_tasksQueueLocker = 0;
	_tasksQueue = 0;
}

/*
 * Constructor
 */
Thread::Thread(int threadId, queue<ITask*>* tasksQueue, pthread_mutex_t* tasksQueueLocker){
	_tasksQueue = tasksQueue;
	_tasksQueueLocker = tasksQueueLocker;

	_threadId = threadId;
	_shouldRun = true;
	_isTasking = false;
}

/*
 * Creates and return this wrapper pthread_t
 */
pthread_t* Thread::create(){
	threadData* data = new threadData;
	data->_threadId = _threadId;
	data->_shouldRun = &_shouldRun;
	data->_isTasking = &_isTasking;
	data->_tasksQueue = _tasksQueue;
	data->_tasksQueueLocker = _tasksQueueLocker;

	pthread_create(&_threadId, NULL, run, (void*)data);

	return &_threadId;
}

/*
 * Destructor
 */
Thread::~Thread() {
	_shouldRun = false;
	pthread_join(_threadId,NULL);
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
	threadData* thisThread = (threadData*)args;
	pthread_mutex_t* locker = thisThread->_tasksQueueLocker;
	queue<ITask*>* tasksQueue = thisThread->_tasksQueue;

	while(*(thisThread->_shouldRun)){

		pthread_mutex_lock(locker);
		if(!tasksQueue->empty()){
			taskToHandle = tasksQueue->front();
			tasksQueue->pop();		
			pthread_mutex_unlock(locker);

			/*
			 * Task holds everything it needs.
			 * Next iteration of this thread
			 * will only happen after this task ends.
			 */

			if(taskToHandle != 0){
				*(thisThread->_isTasking) = true;
				taskToHandle->start();
				*(thisThread->_isTasking) = false;
				delete taskToHandle;
			}
		}else{
			pthread_mutex_unlock(locker);
		}
	}

	tasksQueue = 0;
	locker = 0;
	taskToHandle = 0;
	thisThread->_shouldRun = 0;
	thisThread->_isTasking = 0;
	thisThread->_tasksQueue = 0;
	thisThread->_tasksQueueLocker = 0;

	delete thisThread;
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
