//============================================================================
// Name        : Logger.cpp
// Author      : Alon Aviv Uriah Ahrak
// Description : This is our customized Logger class.
//				 While adding libraries is more trivial we have figured that
//				 writing our own Logger is much more interesting.
//				 Some of 'unique' properties and features:
//				 The ability to both print (shouldPrint?) and save log to file.
//============================================================================
#include "Logger.h"

// Initialize class static member outside of class scope
Logger* Logger::_instance = 0;

/*
 * Private method to get current time in a string
 * in order to keep Log updated and accurate -
 * time-wise
 */
string getCurrentTime();

/*
 * Writing a message to log, debug mode.
 */
void Logger::debug(const string& msg){
	write(DEBUG, msg);
}
/*
 * Writing a message to log, warn mode.
 */
void Logger::warn(const string& msg){
	write(WARN, msg);
}
/*
 * Writing a message to log, info mode.
 */
void Logger::info(const string& msg){
	write(INFO, msg);
}

/*
 * Logger saves the log to a file by default.
 * If shouldPrint gets TRUE as an argument the logger
 * will also print to cout ostream the messages it gets
 * in our Logger messages format.
 */
void Logger::shouldPrint(bool value){
	_shouldPrint = value;
}
/*
 * Private method to put messages in format.
 * This method saves user's trouble to send an extra argument of Level identifier.
 */
void Logger::write(Logger_Levels level, const string& msg){
	// Initialize stream to write to
	stringstream msgInFormat;
	msgInFormat.clear();
	msgInFormat.flush();

	// Format is <time> <level>: <message>
	msgInFormat << getCurrentTime()
			    << Logger_Levels_Str[level]
				<<": " << msg << endl;

	//_fStream_mutex.lock();
	pthread_mutex_lock(&_fStream_mutex);
	// Write the message in Loggers format to fStream
	_fStream << msgInFormat.str();
	pthread_mutex_unlock(&_fStream_mutex);
	//_fStream_mutex.unlock();

	// Print it out to user, if asked to
	if(_shouldPrint){
		cout << msgInFormat.str();
	}
}
/*
 * Private method to get current time in a string
 * in order to keep Log updated and accurate -
 * time-wise.
 * Took this part from the Internet.
 */
string getCurrentTime(){
	// Get time now
	time_t t = time(0);
	struct tm * now = localtime(&t);

	stringstream currentTime;
	currentTime << now->tm_mday << '/'
		 << (now->tm_mon + 1) << '/'
		 << (now->tm_year + 1900) << ' '
		 << now->tm_hour << ':'
		 << now->tm_min << ':'
		 << now->tm_sec << ' ';

	// Time format is dd/mm/yyyy
	return currentTime.str();
}
/*
 * Destructor
 */
Logger::~Logger() {
	//_fStream_mutex.lock();
	pthread_mutex_lock(&_fStream_mutex);
	_fStream.flush();
	_fStream.close();
	pthread_mutex_unlock(&_fStream_mutex);
	pthread_mutex_destroy(&_fStream_mutex);
//	_fStream_mutex.unlock();
}
/*
 * Private method to create headers in log in our
 * customized format.
 */
void Logger::createHeadLine(){
	stringstream msgInFormat;
	msgInFormat.clear();
	msgInFormat.flush();
	msgInFormat << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	msgInFormat << "*  Advanced Programming Logger *"<< endl;
	msgInFormat << "* Customized by Alon and Uriah *"<< endl;
	msgInFormat << "*      " << getCurrentTime() << "       *" << endl;
	msgInFormat << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

	//_fStream_mutex.lock();
	pthread_mutex_lock(&_fStream_mutex);
	_fStream << msgInFormat.str();
	pthread_mutex_unlock(&_fStream_mutex);
	//_fStream_mutex.unlock();

	// Print to user, if asked to
	if(_shouldPrint){
		cout << msgInFormat.str();
	}
}

Logger& Logger::operator = (const Logger& newAssign){
	if(this == &newAssign){
		return *this;
	}
	delete _instance;
	_instance = new Logger();
	return *this;
}
