/* Datatypes and datatypes of function blocks */
typedef enum LoggerStep_enum
{	LOGGER_WAIT,
	LOGGER_GET_INFO_LOG,
	LOGGER_CREATE_LOG,
	LOGGER_WRITE
} LoggerStep_enum;

typedef struct LoggerEntry_type
	{	
	unsigned char LogLevel;
	long int ErrorNumber;
	char AsciiData[257];
} LoggerEntry_type;

typedef struct LoggerFifo_type
	{	
	unsigned char PendingEntries;
	struct LoggerEntry_type Entry[10];
} LoggerFifo_type;

typedef struct LoggerHandler_type
	{	signed long ErrorInternal;
	char LoggerName[11];
	enum LoggerStep_enum LogStep;
	struct LoggerFifo_type LogFifo;
	struct LoggerEntry_type LogEntry;
	unsigned long LogIdent;
	struct ArEventLogGetIdent ArEventLogGetIdent_0;
	struct ArEventLogCreate ArEventLogCreate_0;
	struct ArEventLogWrite ArEventLogWrite_0;
	plcbit LastExecute;
} LoggerHandler_type;



