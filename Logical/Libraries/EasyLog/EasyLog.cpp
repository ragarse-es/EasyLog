#include "Logger.h"
#include "EasyLog.h"
#include "LoggerInternal.hpp"

//extern "C" {
long int EasyLog(plcbit Execute, plcstring* LoggerName, unsigned char LogLevel, unsigned short ErrorNumber, plcstring* Message) 
{
	Logger& logger = Logger::getInstance();
	return logger.EasyLogInternal(Execute, LoggerName, LogLevel, ErrorNumber, Message);
}
//}