
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "EasyLog.h"
#ifdef __cplusplus
	};
#endif

#include "GlobalIncludes.hpp"
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
static std::map<std::string, unsigned long> internalMap;

//Functions to get ident and write. As long as this functions have syncronous
// execution does not have any sense save one copy of each function in the handler
struct ArEventLogGetIdent ArEventLogGetIdent_0;
struct ArEventLogWrite ArEventLogWrite_0;

/**
 * Retrieves or creates the internal data structure for the given instance.
 *
 * This function checks if an internal data structure for the provided instance
 * exists in the internal map. If it does not, it initializes a new internal
 * structure and stores it in the map.
 */
unsigned long SearchMap(unsigned long inst){
	
	//Retrieve the logger name
	std::string loggerName((char*)inst);	
	std::map<std::string, unsigned long>::iterator it;
	
	//Search the Handler with the logger name
	it = internalMap.find(loggerName);
	
	//If it is not found, create a new Handler to insert to the map with the new logger name
	if (it == internalMap.end())
	{
		//Local instance of the handler
		struct LoggerHandler_type newHandler = {0};		
		newHandler.LoggerName[11] = {0};		
		std::strncpy(newHandler.LoggerName, loggerName.c_str(), sizeof(newHandler.LoggerName) - 1);	
		
		//Create a local instance of queue, allocate it in memory and 
		//achieve the pointer to that queue to store it in the local handler
		std::queue<LoggerEntry_type>* localLoggerQueue = (std::queue<LoggerEntry_type>*)malloc(sizeof(std::queue<LoggerEntry_type>));
		new(localLoggerQueue) std::queue<LoggerEntry_type>();
		newHandler.pLoggerQueue = reinterpret_cast<unsigned long>(localLoggerQueue);
		
		//Allocate the Handler in memory and save the local instance)
		LoggerHandler_type* savedInstance = (LoggerHandler_type*)malloc(sizeof(LoggerHandler_type));
			
		//Copy the local instance to the saved memory
		brsmemcpy((UDINT)savedInstance, (UDINT)&newHandler, sizeof(LoggerHandler_type));		
		
		//Store the pointer of the saved instance
		unsigned long pHandler = reinterpret_cast<unsigned int>(savedInstance);
		
		//Save the new instance in the map and return it
		internalMap[loggerName] = pHandler;		
		return internalMap[loggerName];	
	} 
	//If it is found return the Handler
	else
	{
		return it->second;
	}
}

/* Function to execute when is necessary a log insertion. 
	It will be enqueued in a Queue until the UserLogWrite inserts the messages in the logger*/
long int UserLogEnqueue(unsigned char LogLevel, unsigned short ErrorNumber, unsigned long pAsciiData, unsigned long pLoggerName, unsigned long pHandler)
{
	//Retrieve the Handler and its queue through its pointers
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	std::queue<LoggerEntry_type>* queue = reinterpret_cast<std::queue<LoggerEntry_type>*>(Handler->pLoggerQueue);
	
	long int Status = ERR_OK;
	
	//Create a new entry type with the external information
	LoggerEntry_type newEntry;
	newEntry.LogLevel = LogLevel;
	newEntry.ErrorNumber = ErrorNumber;
	brsmemcpy((UDINT)&newEntry.AsciiData, pAsciiData, sizeof(newEntry.AsciiData));

	//Push that entry in the queue  
	queue->push(newEntry);
	
	return Status;
}

//Function to execute asyncronously with the Logger name to insert 
// the messages in the logger that has been inserted with the UserLogEnqueue function
long int UserLogWrite(unsigned long pHandler, bool fromAsync)
{
	//Retrieve the Handler and its queue through its pointers
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);	
	std::queue<LoggerEntry_type>* queue = reinterpret_cast<std::queue<LoggerEntry_type>*>(Handler->pLoggerQueue);
	
	//Initialize the Status
	long int Status = ERR_OK;			
	Handler->ErrorInternal = ERR_OK;
	
	//If it is not busy the creation of the logger
	if (!Handler->CreatingLogger || fromAsync) 
	{			
		//Check if it is necessary to write the log
		if (queue->size() > 0 && Handler->LogIdent != 0)
		{		
			int elements = queue->size();
			for (int i = 0; i < elements; i++)
			{	
				// Put the first entry in the Handler		
				Handler->LogEntry = queue->front();
		
				//Set the parameters of the write function
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
					//Reset the function
					ArEventLogWrite_0.Execute = 0;
					ArEventLogWrite(&ArEventLogWrite_0);
					
					if (queue->size()>0)
					{	  
						//Delete the fist message
						queue->pop();
					}
					
					Status = ERR_OK;
				} 
					//Error case
				else if (ArEventLogWrite_0.Error) 
				{
					//Get the error
					Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : ArEventLogWrite_0.StatusID;
					Status = ArEventLogWrite_0.StatusID;	
		
					//Reset the function
					ArEventLogWrite_0.Execute = 0;			
					ArEventLogWrite(&ArEventLogWrite_0);				
				}
			}
		}			
	}
	
	Handler->ErrorInternal = Status;
	
	return Status;
}

//Function to execute asyncronously to create a new logger if it does not exist
void CreateNewLoggerAsync(void *Params)
{	
	// Recast Parameters to LoggerHandler_type*
	unsigned long  pHandler = reinterpret_cast<unsigned long>(Params);		
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	//Parameters of the new Logger
	Handler->ArEventLogCreate_0.Execute = 1;
	brsstrcpy((UDINT)&Handler->ArEventLogCreate_0.Name, (UDINT)&Handler->LoggerName);
	Handler->ArEventLogCreate_0.Size = 200000;
	Handler->ArEventLogCreate_0.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;
		
	//Execute as many times it takes to finish, ok or with errors
	do
	{
	  ArEventLogCreate(&Handler->ArEventLogCreate_0);
	} while (!Handler->ArEventLogCreate_0.Done && !Handler->ArEventLogCreate_0.Error);
		                     
	//Created OK, Reset the create function and write the pending entries
	if (Handler->ArEventLogCreate_0.Done) 
	{
		//Save the logger ident
		Handler->LogIdent = Handler->ArEventLogCreate_0.Ident;	
		
		//Reset function
		Handler->ArEventLogCreate_0.Execute = 0;
		ArEventLogCreate(&Handler->ArEventLogCreate_0);	
		
		//Write pending entries
		UserLogWrite(pHandler, 1);
		Handler->CreatingLogger = 0;
	} 
		
	//Error case. Reset the create function and inform the error
	else if (Handler->ArEventLogCreate_0.Error) 
	{		
		Handler->CreatingLogger = 0;
		
		//Get the error		
		Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogCreate_0.StatusID;		
		
		//Reset the function
		Handler->ArEventLogCreate_0.Execute = 0;						
		ArEventLogCreate(&Handler->ArEventLogCreate_0);		
	}	
	
	//Delete this task once finished
	RtkDeleteTask(RtkGetCurrentTaskIdent());	
}

long int UserLogCreateLog(unsigned long pHandler)
{
	//Retrieve the Handler through its pointer
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);
	
	//Initialize the Status
	long int Status = ERR_OK;
	if (!Handler->CreatingLogger) 
	{
		//Launch the asyncronous task to create the logger if necessary
		if (Handler->CreateLogger && Handler->CreatingLogger == 0)
		{	
			Handler->CreatingLogger = 1;	
			LPRTK_CREATE_TASK_FKT function = &CreateNewLoggerAsync;
			const char* taskName =  Handler->LoggerName;
			void* pHandlerVoid = reinterpret_cast<void*>(pHandler);
			RtkCreateTask(taskName, 10, 0x1000, 0x1000, RTK_TASK_RESUMED, function, pHandlerVoid, &Handler->TaskHandler); 
			
		}
	}
	
	return Status;
}

//Function to execute asyncronously with the Logger name to insert 
// the messages in the logger that has been inserted with the UserLogEnqueue function
long int UserLogGetIdent(unsigned long pHandler)
{
	//Retrieve the Handler and its queue through its pointers
	struct LoggerHandler_type* Handler = reinterpret_cast<LoggerHandler_type*>(pHandler);	
	std::queue<LoggerEntry_type>* queue = reinterpret_cast<std::queue<LoggerEntry_type>*>(Handler->pLoggerQueue);
	
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
		
			//Finded the logger
			if (ArEventLogGetIdent_0.Done) 
			{
				//Save the logger ident in the handler
				Handler->LogIdent = ArEventLogGetIdent_0.Ident;	
				
				//Reset the function
				ArEventLogGetIdent_0.Execute = 0;
				ArEventLogGetIdent(&ArEventLogGetIdent_0);	
				
				if (queue->size() > 0)
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
				//Flag to star the creation of the logger
				Handler->CreateLogger = 1;
				
				//Reset the function
				ArEventLogGetIdent_0.Execute = 0;
				ArEventLogGetIdent(&ArEventLogGetIdent_0);
				Status = ERR_FUB_BUSY;
			} 
			//Error case
			else if (ArEventLogGetIdent_0.Error) 
			{
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : ArEventLogGetIdent_0.StatusID;
				
				//Get the error
				Status = ArEventLogGetIdent_0.StatusID;
				
				//Reset the function
				ArEventLogGetIdent_0.Execute = 0;
				ArEventLogGetIdent(&ArEventLogGetIdent_0);
			}
		}
	}
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
		
	//Search the handler in the map
	unsigned long pHandler = SearchMap(pLoggerName);
	
	//Command to insert a new message
	if (Execute)
	{
		//Check that the parameters are correct when an insertion is needed
		if (strcmp(Message, "") == 0 || strcmp(LoggerName, "") == 0 || LogLevel < 0 || LogLevel > 3)
		{
			//Wrong parameters
			Status = ERR_BUR_ILLPAR;
		}
		else
		{
			//Enqueue the message in the Queue
			StatusEnqueue = UserLogEnqueue(LogLevel, ErrorNumber, pMessage, pLoggerName, pHandler);			
		}
	}

	//Write the pending messages (or create the logger in the first insertion)
	if (strcmp(LoggerName, "") != 0)
	{	 		
		UserLogGetIdent(pHandler);
		UserLogCreateLog(pHandler);		
		StatusWrite = UserLogWrite(pHandler, 0); 
	}
	else		
	{	
		//Necessary logger name
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


