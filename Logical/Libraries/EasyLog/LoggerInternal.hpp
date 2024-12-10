#ifndef LOGGERINTERNAL_H
#define LOGGERINTERNAL_H

#include "ArEventLog.h"
#include <stdlib.h>
#include "AsBrStr.h"
#include "runtime.h"
#include "sys_lib.h"
#include <iostream>
#include <cstring>
#include <string>
#include <queue>
#include <map>
#include "rtk.h"

struct LoggerEntry_type 
{
	unsigned char LogLevel;
	long int ErrorNumber;
	char AsciiData[257];
};

struct LoggerHandler_type
{
	signed long ErrorInternal;
	char LoggerName[11];
	unsigned long pLoggerQueue;
	struct LoggerEntry_type LogEntry;
	unsigned long LogIdent;
	struct ArEventLogCreate ArEventLogCreate_0;
	RTK_TASK_H TaskHandler;
	bool CreatingLogger;
	bool CreateLogger;
};



class Logger {
public:
    static Logger& getInstance();
	long int EasyLogInternal(plcbit Execute, plcstring* LoggerName, unsigned char LogLevel, unsigned short ErrorNumber, plcstring* Message);

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;


	//Headers of the functions
    unsigned long SearchMap(unsigned long inst);
    long int UserLogEnqueue(unsigned char LogLevel, unsigned short ErrorNumber, unsigned long pAsciiData, unsigned long pLoggerName, unsigned long pHandler);
    long int UserLogWrite(unsigned long pHandler, bool fromAsync);
    long int UserLogCreateLog(unsigned long pHandler);
    long int UserLogGetIdent(unsigned long pHandler);
    static void CreateNewLoggerAsync(void* Params);	

};

#endif // LOGGERINTERNAL_H



