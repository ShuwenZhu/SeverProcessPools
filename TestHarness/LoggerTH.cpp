#include "LoggerTH.h"
#include <iostream>
#include <iomanip>
#include <ctime>    
#include <sstream>

#pragma warning(disable : 4996)

using std::cout;
using std::endl;
using std::setw;
using std::left;

const int headerLevel = 8;

LoggerTH::LoggerTH(Level l) {
	level = l;
}

LoggerTH::LoggerTH() {
	level =info;
}

void LoggerTH::setLogLevel(Level l) {
	level = l;
}

// The next few functions are our multi level printers. They only print when
// the level (or higher) matches, and formats the output for us.
//===================================================================
void LoggerTH::printer(Level myLevel, std::string str) {
	std::string levelHeader = "[Unkonwn]";
	std::string timestamp;
	std::ostringstream timeStream;
	std::time_t t = std::time(0);   // get time now
	std::tm* now = std::localtime(&t);

	timeStream << "[" << 1900 + now->tm_year << "-"
		<< 1 + now->tm_mon << "-"
		<< now->tm_mday << " "
		<< 1 + now->tm_hour << ":"
		<< 1 + now->tm_min << ":"
		<< 1 + now->tm_sec << "]";
	timestamp = timeStream.str();

	switch (myLevel) {
	case critical: levelHeader = "[Crit] "; break;
	case error:    levelHeader = "[Error] "; break;
	case warning:  levelHeader = "[Warn] "; break;
	case info:     levelHeader = "[Info] "; break;
	case debug:    levelHeader = "[Debug] "; break;
	}
	cout << setw(headerLevel) << left << levelHeader << timestamp <<  " : "
		 <<  str << endl;
}

// Critical always prints.
void LoggerTH::Critical(std::string str) {
	printer(critical, str);
}

// Error only prints critical and error messages
void LoggerTH::Error(std::string str) {
	if (level <= error) {
		printer(error, str);
	}
}

// Warning will print Warning on down.
void LoggerTH::Warning(std::string str) {
	if (level <= warning) {
		printer(warning, str);
	}
}

// info will print Info on down.
void LoggerTH::Info(std::string str) {
	if (level <= info) {
		printer(info, str);
	}
}

// Debug only prints debug marked statements.
void LoggerTH::Debug(std::string str) {
	if (level == debug) {
		printer(debug, str);
	}
}
//===================================================================