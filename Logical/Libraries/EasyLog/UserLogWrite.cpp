
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "EasyLog.h"
#include "GlobalIncludes.h"
#ifdef __cplusplus
	};
#endif


#include "ArEventLog.h"
#include <stdlib.h>
#include "AsBrStr.h"
#include "runtime.h"
#include "sys_lib.h"
#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include "rtk.h"

#define sdRB_ERR_ASSERT 1001

/**
 * Internal Data Map
 *
 * This map is used to store and retrieve the internal state for each instance, allowing
 * for the management of multiple instances within the system. Related a Handler with all the information
with one logger name given as a string
 */
//static std::map<std::string, struct LoggerHandler_type> internalMap;
static std::map<std::string, unsigned long> internalMap;

struct ArEventLogGetIdent ArEventLogGetIdent_0;
struct ArEventLogWrite ArEventLogWrite_0;

long int UserLogWrite(unsigned long pHandler);

static bool creatingLogger;

static WORD priority = 10;

/**
 * Retrieves or creates the internal data structure for the given instance.
 *
 * This function checks if an internal data structure for the provided instance
 * exists in the internal map. If it does not, it initializes a new internal
 * structure and stores it in the map.
 */
unsigned long SearchMap(unsigned long inst){
	
	std::string loggerName((char*)inst);	
	std::map<std::string, unsigned long>::iterator it;
	
	//Search the Handler with the logger name
	it = internalMap.find(loggerName);
	
	//If it is not found, create a new Handler to insert to the map with the new logger name
	if (it == internalMap.end())
	{
		struct LoggerHandler_type newHandler = {0};		
		newHandler.LoggerName[11] = {0};		
		std::strncpy(newHandler.LoggerName, loggerName.c_str(), sizeof(newHandler.LoggerName) - 1);	
		
		//Allocate the memory ans save the local instance)
		LoggerHandler_type* savedInstance = (LoggerHandler_type*)malloc(sizeof(LoggerHandler_type));
			
			//Copy the local instance to the saved memory
			*savedInstance = newHandler;
		
		//Store the pointer of the saved ins
		unsigned long pHandler = reinterpret_cast<unsigned int>(savedInstance);
		
		internalMap[loggerName] = pHandler;
		return internalMap[loggerName];	
	} 
		//If it is fount return the Handler
	else
	{
		return it->second;
	}
}



/* Function to execute when is necessary a log insertion. 
	It will be enqueued in a Fifo until the UserLogWrite inserts the messages in the logger*/
long int UserLogEnqueue(unsigned char LogLevel, unsigned short ErrorNumber, unsigned long pAsciiData, unsigned long pLoggerName, unsigned long pHandler)
{
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	long int Status = ERR_OK;
		
	if (Handler->LogFifo.PendingEntries < sizeof(Handler->LogFifo.Entry) / sizeof(Handler->LogFifo.Entry[0])) {
		Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].LogLevel = LogLevel;
		Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].ErrorNumber = ErrorNumber;
		brsmemcpy((UDINT)&Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].AsciiData, pAsciiData, sizeof(Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].AsciiData));
		if (brsstrlen((UDINT)&Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].AsciiData) >= sizeof(Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].AsciiData)) {
			brsstrcpy((UDINT)&Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].AsciiData + sizeof(Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries].AsciiData) - 4, (UDINT)&"...");
		}
		Handler->LogFifo.PendingEntries++;
	} 
	else if (Handler->LogFifo.PendingEntries < sizeof(Handler->LogFifo.Entry) / sizeof(Handler->LogFifo.Entry[0])) {
		Status = ERR_FUB_BUSY;
	}
	return Status;
}

//Function to execute asyncronously to create a new logger if it does not exist
void CreateNewLoggerAsync(void *Params)
{	
	// Recast Parameters to LoggerHandler_type*
	unsigned long  pHandler = reinterpret_cast<unsigned long>(Params);
		
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	//Status = ERR_FUB_BUSY;
	//Parameters of the new Logger
	Handler->ArEventLogCreate_0.Execute = 1;
	brsstrcpy((UDINT)&Handler->ArEventLogCreate_0.Name, (UDINT)&Handler->LoggerName);
	Handler->ArEventLogCreate_0.Size = 200000;
	Handler->ArEventLogCreate_0.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;
		
	do
	{
	  ArEventLogCreate(&Handler->ArEventLogCreate_0);
	} while (!Handler->ArEventLogCreate_0.Done && !Handler->ArEventLogCreate_0.Error);
		                     
	//Finished LogCreate. Move forward
	if (Handler->ArEventLogCreate_0.Done) 
	{
		Handler->ArEventLogCreate_0.Execute = 0;
		Handler->LogIdent = Handler->ArEventLogCreate_0.Ident;				
		ArEventLogCreate(&Handler->ArEventLogCreate_0);			
		Handler->CreatingLogger = 0;
		//Once created, call the function to insert the pending entries
		UserLogWrite(pHandler);
	} 
	//Error case
	else if (Handler->ArEventLogCreate_0.Error) 
	{		
		Handler->CreatingLogger = 0;
		Handler->ArEventLogCreate_0.Execute = 0;
		Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogCreate_0.StatusID;		
		//Status = Handler->ArEventLogCreate_0.StatusID;							
		ArEventLogCreate(&Handler->ArEventLogCreate_0);		
	}		
	//Delete the task once finished
	RtkDeleteTask(RtkGetCurrentTaskIdent());	
}


//Function to execute asyncronously with the Logger name to insert 
// the messages in the logger that has been inserted with the UserLogEnqueue function
long int UserLogGetIdent(unsigned long pHandler)
{
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	
	//Initialize the Status
	long int Status = ERR_OK;	
	Handler->CreateLogger = 0;
	
	//Only search the logger if the log ident does not exist
	if (Handler->LogIdent == 0)
	{
	
		//If it is not busy the creation of the logger
		if (!Handler->CreatingLogger) 
		{	

			//Get the info log
			ArEventLogGetIdent_0.Execute = 1;
			brsstrcpy((UDINT)ArEventLogGetIdent_0.Name, (UDINT)&Handler->LoggerName);   
			ArEventLogGetIdent(&ArEventLogGetIdent_0);
		
			//Finished getIdent, move forward
			if (ArEventLogGetIdent_0.Done) 
			{
				ArEventLogGetIdent_0.Execute = 0;
				Handler->LogIdent = ArEventLogGetIdent_0.Ident;		
				ArEventLogGetIdent(&ArEventLogGetIdent_0);			
				if (Handler->LogFifo.PendingEntries > 0)
				{		
					Status = ERR_FUB_BUSY;
				}
				else
				{
					Status = ERR_OK;
				}
			} 
				//First appearing of the logger, necessary creation
			else if (ArEventLogGetIdent_0.StatusID == arEVENTLOG_ERR_LOGBOOK_NOT_FOUND) 
			{
				ArEventLogGetIdent_0.Execute = 0;
				Handler->CreateLogger = 1;
				ArEventLogGetIdent(&ArEventLogGetIdent_0);
				Status = ERR_FUB_BUSY;
			} 
				//Error case
			else if (ArEventLogGetIdent_0.Error) 
			{
				ArEventLogGetIdent_0.Execute = 0;
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : ArEventLogGetIdent_0.StatusID;			
				ArEventLogGetIdent(&ArEventLogGetIdent_0);
				Status = ArEventLogGetIdent_0.StatusID;
			}
		}
	}
	return Status;
}

long int UserLogCreateLog(unsigned long pHandler)
{
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	//Initialize the Status
	long int Status = ERR_OK;
	if (!Handler->CreatingLogger) 
	{
		//Launch the asyncronous task to create the logger
		if (Handler->CreateLogger && Handler->CreatingLogger == 0)
		{	
			Handler->CreatingLogger = 1;	
			LPRTK_CREATE_TASK_FKT function = &CreateNewLoggerAsync;
			const char* taskName =  Handler->LoggerName;
			void* pHandlerVoid = reinterpret_cast<void*>(pHandler);
			RtkCreateTask(taskName, priority, 0x1000, 0x1000, RTK_TASK_RESUMED, function, pHandlerVoid, &Handler->TaskHandler); 
			priority++;
		}
	}
	
	return Status;
}

//Function to execute asyncronously with the Logger name to insert 
// the messages in the logger that has been inserted with the UserLogEnqueue function
long int UserLogWrite(unsigned long pHandler)
{
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	//Initialize the Status
	long int Status = ERR_OK;			
	Handler->ErrorInternal = ERR_OK;
	
	// Local temporary variables which are used in many places
	UDINT i;	
	
	//If it is not busy the creation of the logger
	if (!Handler->CreatingLogger) 
	{	
		
		//Check if it is necessary to write the log
		if (Handler->LogFifo.PendingEntries > 0 && Handler->LogIdent != 0)
		{		
			
			for (int i = 0; i < Handler->LogFifo.PendingEntries; i++)
			{	
				// Put the first entry in the Handler
				Handler->LogEntry = Handler->LogFifo.Entry[0];	
		
		
					
				Status = ERR_FUB_BUSY;			
				ArEventLogWrite_0.Execute = 1;
				ArEventLogWrite_0.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
				ArEventLogWrite_0.AddDataSize = brsstrlen((UDINT)&Handler->LogEntry.AsciiData) + 1;
				ArEventLogWrite_0.AddData = (UDINT)&Handler->LogEntry.AsciiData;
				ArEventLogWrite_0.EventID = ArEventLogMakeEventID(Handler->LogEntry.LogLevel,0,Handler->LogEntry.ErrorNumber);
				brsstrcpy((UDINT)ArEventLogWrite_0.ObjectID, (UDINT)&Handler->LoggerName);
				ArEventLogWrite_0.Ident = Handler->LogIdent;
				ArEventLogWrite(&ArEventLogWrite_0);
				
				//Written OK. Quit from the queue and finish the process
				if (ArEventLogWrite_0.Done) 
				{
					ArEventLogWrite_0.Execute = 0;
					ArEventLogWrite(&ArEventLogWrite_0);
					
					//Quit the entry of the Fifo once is writted
					for (i = 0; i < (UDINT)Handler->LogFifo.PendingEntries - 1; i++) 
					{
						Handler->LogFifo.Entry[i] = Handler->LogFifo.Entry[i+1];
					}
					brsmemset((UDINT)&Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries-1], 0 , sizeof(Handler->LogFifo.Entry[0]));
					Handler->LogFifo.PendingEntries--;
					
					Status = ERR_OK;
				} 
					//Error case
				else if (ArEventLogWrite_0.Error) 
				{
					ArEventLogWrite_0.Execute = 0;
					Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : ArEventLogWrite_0.StatusID;
					Status = ArEventLogWrite_0.StatusID;				
					ArEventLogWrite(&ArEventLogWrite_0);				
				}
			}
		}			
	}
	
	Handler->ErrorInternal = Status;
	
	return Status;
}



/* Function to insert messages in custom loggers in an aeasy way. The Executed is detected by Edge*/
long int EasyLog(plcbit Execute, plcstring* LoggerName, unsigned char LogLevel, unsigned short ErrorNumber, plcstring* Message)
{
	//Initialize the different status
	long int Status = ERR_OK;	
	long int StatusEnqueue = ERR_OK;
	long int StatusWrite = ERR_OK;
		
	//Get the pointer to the logger name
	char* pCharMessage = Message;
	unsigned long pMessage = reinterpret_cast<unsigned long>(pCharMessage);
	
	//Get the pointer to the message
	char* pCharLoggerName = LoggerName;
	unsigned long pLoggerName = reinterpret_cast<unsigned long>(pCharLoggerName);
		
	//Search the map to check the last execute
	unsigned long pHandler = SearchMap(pLoggerName);
	
	//Command to insert a new message
	if (Execute)
	{
		//Check that the parameters are correct when an insertion is needed
		if (strcmp(Message, "") == 0 || strcmp(LoggerName, "") == 0 || LogLevel < 0 || LogLevel > 3)
		{
			Status = ERR_BUR_ILLPAR;
		}
		else
		{
			//Enqueue the message in the Fifo
			StatusEnqueue = UserLogEnqueue(LogLevel, ErrorNumber, pMessage, pLoggerName, pHandler);			
		}
	}

	//Write the pending messages (or create the logger in the first insertion)
	if (strcmp(LoggerName, "") != 0)
	{	 		
		UserLogGetIdent(pHandler);
		UserLogCreateLog(pHandler);		
		StatusWrite = UserLogWrite(pHandler); 
	}
	else		
	{		
		Status = ERR_BUR_ILLPAR;
	}
	
	
	//Check the errors
	if (Status == ERR_OK)
	{
		if (StatusEnqueue != ERR_OK)
		{
			Status = StatusEnqueue;	  
		}
		if(StatusWrite != ERR_OK)
		{
			Status = StatusWrite;	
		}				
	}
	
	return Status;
}


