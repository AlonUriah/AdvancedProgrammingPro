//============================================================================
// Name        : IScheduledTask.h
// Author      : Alon Aviv Uriah Ahrak
// Description : This Task handles scheduled Tasks.
//				 The basic logic is:
//				 This task is scheduled (listen to clock)
//				 This task adds itself to ITasker when the time is right
//============================================================================

#include "ITask.h"
#include "ITaskHandler.h"
#include "../Common/Clock.h"
#include "../Common/Scheduled.h"

#ifndef MULTITHREADING_SCHEDULEDTASK_H_
#define MULTITHREADING_SCHEDULEDTASK_H_

class IScheduledTask : public ITask, public Scheduled{
protected:
	int _timeToExecute;
	ITaskHandler* _taskHandler;
	Clock* _clock;

public:
	/*
	 * Constructor with a feature of taskId
	 */
	IScheduledTask(int taskId);

	/*
	 * Sets Clock pointer member to listen to
	 */
	virtual void setClock(Clock* clock);

	/*
	 * Returns this task Clock pointer member
	 */
	virtual Clock* getClock();

	/*
	 * Sets timeToExecute and ITasker to handle task
	 */
	virtual void registerOnTime(int timeToExecute, ITaskHandler* handler);

	/*
	 * Returns time to execute
	 */
	virtual int getTimeToExecture();

	/*
	 * This Task is scheduled and therefore must
	 * implement timePassed.
	 * When time equals timeToExecute this Task adds
	 * itself to ITasker
	 */
	virtual void timePassed(int time);

	/*
	 * This Task is ITask and must implement start
	 */
	virtual void start() = 0;

	/*
	 * Destructor
	 */
	virtual ~IScheduledTask();
};

#endif /* MULTITHREADING_SCHEDULEDTASK_H_ */
