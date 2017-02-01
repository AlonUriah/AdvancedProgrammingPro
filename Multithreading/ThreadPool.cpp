/*
 * ThreadPool.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: alon
 */

#include "ThreadPool.h"
#include "../Common/Logger.h"
#include "unistd.h"
#include <iostream>

using namespace std;

/*
 * Constructor
 */
ThreadPool::ThreadPool(int threadsNum) :
	_threadsNum(threadsNum) {
	_taskQueue = new queue<ITask*>;
	_threadsArr = new Thread*[_threadsNum];
	_shouldRun = false;
	_logger = Logger::getInstance();

	pthread_mutex_init(&_taskQueueLocker,0);

	for(int i=0; i < _threadsNum; i++){
		_threadsArr[i] = new Thread(i, _taskQueue, &_taskQueueLocker);
	}
}

/*
 * Starts thread pool by calling create on each Thread
 */
void ThreadPool::start(){
	_shouldRun = true;
 
	// Start all threads and expose them as pending
	for(int i=0; i<_threadsNum; i++){
		_threadsArr[i]->create();
	}
}

/*
 * Stops Threads running.
 * In case of foceStop, kill threads even if it runs.
 * In case of foreceStop=false, let thread pool finish its tasks
 */
void ThreadPool::stop(bool forceStop){
	if(forceStop || _taskQueue->empty()){
		_shouldRun = false;

		for(int i=0; i<_threadsNum; i++){
			_threadsArr[i]->stop();
		}
	}
	else{
		while(!_taskQueue->empty()){
			sleep(1);
		}

		stop(forceStop);
	}
}

/*
 * ITaskHandler implements addTasks and handle it.
 * This function implements Thread Pool Pattern.
 */
void ThreadPool::addTask(ITask* task){
	pthread_mutex_lock(&_taskQueueLocker);
	_taskQueue->push(task);
	pthread_mutex_unlock(&_taskQueueLocker);

	stringstream ss;
	ss << "task {" << task->getTaskId() << "} was added";
	_logger->info(ss.str());
}

/*
 * Wait for tasking Threads to finish tasking
 */
void ThreadPool::wait(){
	for(int i=0; i<_threadsNum; i++){
		Thread* currThread = (*_threadsArr+i);
		while(currThread->isTasking()){
			sleep(1);
		}
	}
}

/*
 * Destructor
 */
ThreadPool::~ThreadPool() {
	_logger = 0;
	// delete threads
	for(int i=0; i<_threadsNum; i++){
		_threadsArr[i]->stop();
		delete _threadsArr[i];
	}
	delete[] _threadsArr;

	// delete ITasks
	ITask* task = NULL;
	while (!this->_taskQueue->empty())
	{
		task = this->_taskQueue->front();
		this->_taskQueue->pop();
		delete task;
	}
	delete this->_taskQueue;

	pthread_mutex_destroy(&_taskQueueLocker);
}
