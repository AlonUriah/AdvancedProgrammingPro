//============================================================================
// Name        : Logger.h
// Author      : Alon Aviv Uriah Ahrak
// Description : This is our customized Logger class.
//				 While adding libraries is more trivial we have figured that
//				 writing our own Logger is much more interesting.
//				 Some of 'unique' properties and features:
//				 The ability to both print (shouldPrint?) and save log to file.
//============================================================================
#include <vector>
#include <iostream>

// Singleton purposes
#include <mutex>
#include <thread>
#include <fstream>
#include <pthread.h>

#ifndef LOGGER_H_
#define LOGGER_H_

#include "Common.h"

using namespace std;

class Logger {
public:
	/*
	 * Writing messages to log in debug mode.
	 * In the future this feature may include different font color (usually gray).
	 * We didn't want to make the user to send an extra argument of 'Level'.
	 * The following are more user friendly.
	 */
	void debug(const string& msg);
	/*
	 * Writing messages to log in warn mode.
	 * In the future this feature may include different font color (usually yellow).
	 * We didn't want to make the user to send an extra argument of 'Level'.
	 * The following are more user friendly.
	 */
	void warn(const string& msg);
	/*
	 * Writing messages to log in info mode.
	 * In the future this feature may include different font color (usually white).
	 * We didn't want to make the user to send an extra argument of 'Level'.
	 * The following are more user friendly.
	 */
	void info(const string& msg);
	/*
	 * Logger saves the log to a file by default.
	 * If shouldPrint gets TRUE as an argument the logger
	 * will also print to cout ostream the messages it gets
	 * in our Logger messages format.
	 */
	void shouldPrint(bool value);
	/*
	 * Singleton Pattern.
	 * This is crucial in this case.
	 * Especially when files are involved.
	 * We don't allow multiple instances of Logger.
	 */
	static Logger* getInstance(){
		// If Logger was not initialized yet.
		if(_instance == 0){
			_instance = new Logger;
			_instance->_shouldPrint = true;
			_instance->_filePath = "logger.log";
			_instance->_fStream_mutex.lock();
			_instance->_fStream.open(_instance->_filePath, ios::app);
			_instance->createHeadLine();
		}

		return _instance;
	}
	/*
	 * In case of Logger assignment, delete one of objects and keep just one.
	 */
	Logger& operator = (const Logger&) = delete;
	/*
	 * In case of Logger assignment, delete one of objects and keep just one.
	 */
	Logger& operator = (const Logger&&) = delete;
	/*
	 * Destructor
	 */
	virtual ~Logger();

private:
	static Logger* _instance;			  // Singleton Pattern
	const string filePath = "logger.log"; // Default destination file
	bool _shouldPrint;  				  // Print messages to ostream and not just to file

	fstream _fStream;					  // File stream to write to
	string _filePath;					  // FilePath to save the log
	mutex _fStream_mutex;				  // Singleton Pattern

	/*
	 * Private method to put messages in format.
	 * This method saves user's trouble to send an extra argument of Level identifier.
	 */
	void write(Logger_Levels level, const string& msg);

	/*
	 * Private constructor - Singleton Pattern.
	 * Initialize Logger members with default values.
	 */
	Logger(){
		_shouldPrint = true;
		fstream _fStream;
		string _filePath;
		mutex _fStream_mutex;

	}
	/*
	 * Private method to create headers in log in our
	 * customized format.
	 */
	void createHeadLine();
};

#endif /* LOGGER_H_ */
