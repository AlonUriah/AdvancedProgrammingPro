/*
 * Logger.h
 *
 *  Created on: Dec 2, 2016
 *      Author: uriah
 */
#include <vector>
#include <iostream>


#ifndef LOGGER_H_
#define LOGGER_H_

using namespace std;
class Logger {
public:
	/*
	 * Constructs a new Logger system.
	 */
	Logger();
	/*
	 * Destructs the logger system.
	 */
	virtual ~Logger();

	/*
	 * Saves the current log system.
	 */
	void SaveLog();
	/*
	 * Opens a log file.
	 */
	vector<char*>* OpenLog(const char* path);
};

#endif /* LOGGER_H_ */
