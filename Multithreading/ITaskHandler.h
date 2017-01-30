//============================================================================
// Name        : ITaskHandler.h
// Author      : Alon Aviv Uriah Ahrak
// Description : Each ITaskHandler knows to handle tasks by adding them
//				 and calling their start functions
//============================================================================
#include "ITask.h"

#ifndef ITASK_HANDLER_H_
#define ITASK_HANDLER_H_

class ITaskHandler{
public:
	/*
	 * add Tasks to ITask Handler
	 */
	virtual void addTask(ITask* task)=0;

	/*
	 * Destructor
	 */
	virtual ~ITaskHandler(){ };
};

#endif
