#ifndef _LOGGERTH_H_
#define _LOGGERTH_H_

#include <iostream>
enum Level { critical = 5, error = 4, warning = 3, info = 2, debug = 1 };

class LoggerTH {
	Level level;
	void printer(Level myLevel, std::string str);

public:
	LoggerTH();
	LoggerTH(Level level);
	void setLogLevel(Level level);
	void Critical(std::string str);
	void Error(std::string str);
	void Warning(std::string str);
	void Info(std::string str);
	void Debug(std::string str);
};

#endif // !_LOGGERTH_H_