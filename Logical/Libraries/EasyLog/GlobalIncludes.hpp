#include "rtk.h"
#include <queue>
#include <list>

/* Datatypes and datatypes of function blocks */
typedef struct LoggerEntry_type
	{	
	unsigned char LogLevel;
	long int ErrorNumber;
	char AsciiData[257];
} LoggerEntry_type;


typedef struct LoggerHandler_type
	{	signed long ErrorInternal;
	char LoggerName[11];
	unsigned long pLoggerQueue;
	struct LoggerEntry_type LogEntry;
	unsigned long LogIdent;
	struct ArEventLogCreate ArEventLogCreate_0;
	RTK_TASK_H TaskHandler;
	bool CreatingLogger;
	bool CreateLogger;	
} LoggerHandler_type;



