//============================================================================
// Name        : ITask.h
// Author      : Alon Aviv Uriah Ahrak
// Description : This is ITask interface (~ Base abstract class)
//				 Each Task has an id, return value address (that might be null)
//				 and a logger to notify user about tasks status
//============================================================================
#include <pthread.h>
#include <iostream>
#include <memory.h>

#include "../Common/Logger.h"

using namespace std;

#ifndef MULTITHREADING_TASK_H_
#define MULTITHREADING_TASK_H_

class ITask {
protected:
	int _taskId;
	void* _returnValue;
	Logger* _logger;

public:
	/*
	 * Constructor
	 */
	ITask(int id){
		_taskId = id;
		_logger = Logger::getInstance();
		_returnValue = 0;
	}

	/*
	 * Constructor for a non-void functions
	 */
	ITask(int id, void* returnValue){
		_taskId = id;
		_logger = Logger::getInstance();
		_returnValue = &returnValue;
	}

	/*
	 * Update tasks return value address
	 */
	void setReturnValue(void* retValue){
		_returnValue = retValue;
	}

	/*
	 * Get tasks return value
	 */
	void* getReturnValue(){
		return _returnValue;
	}

	/*
	 * Get tasks id
	 */
	int getTaskId(){
		return _taskId;
	}

	/*
	 * Each task must implement this function.
	 * Thread knows to handle Tasks by calling this function
	 */
	virtual void start()=0;

	/*
	 * Destructor
	 * Avoid dangling pointers by nullify pointer members
	 */
	virtual ~ITask(){
		_returnValue = 0;
		_logger = 0;
	};

	/*
	 * Copy return value to _returnValue which is an address
	 * by using memcpy function
	 */
	virtual void setReturnValue(size_t objectSize, void* returnValue){
		memcpy(_returnValue,returnValue,objectSize);
	}

	virtual bool operator==(ITask& other)const{
		return (_taskId == other._taskId);
	}

	virtual ITask& operator=(ITask& otherTask){
		if(this == &otherTask){
			return *this;	
		}
		_returnValue = otherTask._returnValue;
		_logger = otherTask._logger;
		_taskId = otherTask._taskId;
		return *this;
	}
};

#endif /* MULTITHREADING_TASK_H_ */
