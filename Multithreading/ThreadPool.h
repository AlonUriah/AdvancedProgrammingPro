//============================================================================
// Name        : ThreadPool.h
// Author      : Alon Aviv Uriah Ahrak
// Description : This class manages the treadpool.
//				 It implements ITaskHandler by implementing addTask
//============================================================================

#include <queue>
#include <pthread.h>

#include "ITask.h"
#include "ITaskHandler.h"
#include "Thread.h"
#include "../Common/Logger.h"

#ifndef MULTITHREADING_THREADPOOL_H_
#define MULTITHREADING_THREADPOOL_H_

using namespace std;

class ThreadPool : public ITaskHandler{
private:
	int _threadsNum;
	bool _shouldRun;
	Logger* _logger;

	Thread** _threadsArr;
	queue<ITask*>* _taskQueue;
	pthread_mutex_t _taskQueueLocker;

public:
	/*
	 * Constructor
	 */
	ThreadPool(int threadsNum);

	/*
	 * Starts thread pool by calling create on each Thread
	 */
	void start();

	/*
	 * Stops Threads running.
	 * In case of foceStop, kill threads even if it runs.
	 * In case of foreceStop=false, let thread pool finish its tasks
	 */
	void stop(bool forceStop);

	/*
	 * Wait for tasking Threads to finish tasking
	 */
	void wait();

	/*
	 * ITaskHandler implements addTasks and handle it.
	 * This function implements Thread Pool Pattern.
	 */
	void addTask(ITask* task);

	/*
	 * Destructor
	 */
	virtual ~ThreadPool();
};

#endif /* MULTITHREADING_THREADPOOL_H_ */
