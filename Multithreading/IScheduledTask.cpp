//============================================================================
// Name        : IScheduledTask.h
// Author      : Alon Aviv Uriah Ahrak
// Description : This Task handles scheduled Tasks.
//				 The basic logic is:
//				 This task is scheduled (listen to clock)
//				 This task adds itself to ITasker when the time is right
//============================================================================

#include <iostream>
#include "ITask.h"
#include "IScheduledTask.h"
#include "../Common/Clock.h"

using namespace std;

/*
 * Constructor with a feature of taskId
 */
IScheduledTask::IScheduledTask(int taskId) : ITask(taskId){
	_timeToExecute = -1;
	_taskHandler = 0;
	_clock = 0;
}

/*
 * Sets Clock pointer member to listen to
 */
void IScheduledTask::setClock(Clock* clock){
	_clock = clock;

	clock->addListener(this);
}

/*
 * Returns this task Clock pointer member
 */
Clock* IScheduledTask::getClock(){
	return _clock;
}

/*
 * Sets timeToExecute and ITasker to handle task
 */
void IScheduledTask::registerOnTime(int timeToExecute, ITaskHandler* handler){
	_timeToExecute = timeToExecute;
	_taskHandler = handler;
}

/*
 * Returns time to execute
 */
int IScheduledTask::getTimeToExecture(){
	return _timeToExecute;
}

/*
 * This Task is scheduled and therefore must
 * implement timePassed.
 * When time equals timeToExecute this Task adds
 * itself to ITasker
 */
void IScheduledTask::timePassed(int time){
	if(time == _timeToExecute){
		_taskHandler->addTask(this);
	}
}

/*
 * Destructor
 */
IScheduledTask::~IScheduledTask(){
	_taskHandler = 0;
	_clock = 0;
}
