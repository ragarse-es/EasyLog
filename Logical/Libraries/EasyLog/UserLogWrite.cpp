
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

#define sdRB_ERR_ASSERT 1001

/**
 * Internal Data Map
 *
 * This map is used to store and retrieve the internal state for each instance, allowing
 * for the management of multiple instances within the system. Related a Handler with all the information
with one logger name given as a string
 */
static std::map<std::string, struct LoggerHandler_type> internalMap;


/**
 * Retrieves or creates the internal data structure for the given instance.
 *
 * This function checks if an internal data structure for the provided instance
 * exists in the internal map. If it does not, it initializes a new internal
 * structure and stores it in the map.
 */
struct LoggerHandler_type* SearchMap(unsigned long inst){
	
	std::string loggerName((char*)inst);	
	std::map<std::string, struct LoggerHandler_type>::iterator it;
	
	//Search the Handler with the logger name
	it = internalMap.find(loggerName);
	
	//If it is not found, create a new Handler to insert to the map with the new logger name
	if (it == internalMap.end())
	{
		struct LoggerHandler_type newHandler = {0};		
		newHandler.LoggerName[11] = {0};		
		std::strncpy(newHandler.LoggerName, loggerName.c_str(), sizeof(newHandler.LoggerName) - 1);		
		internalMap[loggerName] = newHandler;
		return &internalMap[loggerName];	
	} 
	//If it is fount return the Handler
	else
	{
		return &it->second;
	}
}



/* Function to execute when is necessary a log insertion. 
	It will be enqueued in a Fifo until the UserLogWrite inserts the messages in the logger*/
long int UserLogEnqueue(unsigned char LogLevel, unsigned short ErrorNumber, unsigned long pAsciiData, unsigned long pLoggerName, struct LoggerHandler_type* Handler)
{
	
	long int Status = ERR_OK;
		
	if (Handler->LogFifo.PendingEntries < sizeof(Handler->LogFifo.Entry) / sizeof(Handler->LogFifo.Entry[0]) - 1) {
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



//Function to execute asyncronously with the Logger name to insert 
// the messages in the logger that has been inserted with the UserLogEnqueue function
long int UserLogWrite(struct LoggerHandler_type* Handler)
{
	//Initialize the Status
	long int Status = ERR_OK;			
	Handler->ErrorInternal = ERR_OK;
	
	// Local temporary variables which are used in many places
	UDINT i;
	BOOL createLog = 0;
	BOOL writeLog = 0;
	
	
	//Check if a logger it is being created
	if (Handler->ArEventLogCreate_0.Busy)
	{
		Status = ERR_FUB_BUSY;
		//Execute th FB 
		ArEventLogCreate(&Handler->ArEventLogCreate_0);
	                     
		if (Handler->ArEventLogCreate_0.Done) 
		{
			Handler->ArEventLogCreate_0.Execute = 0;
			Handler->LogIdent = Handler->ArEventLogCreate_0.Ident;				
			ArEventLogCreate(&Handler->ArEventLogCreate_0);
			Status = ERR_FUB_BUSY;
		} 
		else if (Handler->ArEventLogCreate_0.Error) 
		{
			Handler->ArEventLogCreate_0.Execute = 0;
			Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogCreate_0.StatusID;		
			Status = Handler->ArEventLogCreate_0.StatusID;							
			ArEventLogCreate(&Handler->ArEventLogCreate_0);
		}
		
	}
	
	//If it is not busy the creation of the logger
	if (!Handler->ArEventLogCreate_0.Busy) 
	{
		// Put the first entry in the Handler
		Handler->LogEntry = Handler->LogFifo.Entry[0];		

		//Get the info log
		Handler->ArEventLogGetIdent_0.Execute = 1;
		brsstrcpy((UDINT)&Handler->ArEventLogGetIdent_0.Name, (UDINT)&Handler->LoggerName);   
		ArEventLogGetIdent(&Handler->ArEventLogGetIdent_0);
		
		//Finished getIdent, move forward
		if (Handler->ArEventLogGetIdent_0.Done) 
		{
			Handler->ArEventLogGetIdent_0.Execute = 0;
			Handler->LogIdent = Handler->ArEventLogGetIdent_0.Ident;		
			ArEventLogGetIdent(&Handler->ArEventLogGetIdent_0);			
			if (Handler->LogFifo.PendingEntries > 0)
			{	  
				writeLog = 1;					
				Status = ERR_FUB_BUSY;
			}
			else
			{
				Status = ERR_OK;
			}
		} 
			//First appearing of the logger, necessary creation
		else if (Handler->ArEventLogGetIdent_0.StatusID == arEVENTLOG_ERR_LOGBOOK_NOT_FOUND) 
		{
			Handler->ArEventLogGetIdent_0.Execute = 0;
			createLog = 1;			
			ArEventLogGetIdent(&Handler->ArEventLogGetIdent_0);
			Status = ERR_FUB_BUSY;
		} 
			//Error case
		else if (Handler->ArEventLogGetIdent_0.Error) 
		{
			Handler->ArEventLogGetIdent_0.Execute = 0;
			Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogGetIdent_0.StatusID;			
			ArEventLogGetIdent(&Handler->ArEventLogGetIdent_0);
			Status = Handler->ArEventLogGetIdent_0.StatusID;
		}
		
		//Start the process of the logger creation
		if (createLog)
		{					
			Status = ERR_FUB_BUSY;			
			Handler->ArEventLogCreate_0.Execute = 1;
			brsstrcpy((UDINT)&Handler->ArEventLogCreate_0.Name, (UDINT)&Handler->LoggerName);
			Handler->ArEventLogCreate_0.Size = 200000;
			Handler->ArEventLogCreate_0.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;		
			ArEventLogCreate(&Handler->ArEventLogCreate_0);
	                     
			//Finished LogCreate. Move forward
			if (Handler->ArEventLogCreate_0.Done) 
			{
				Handler->ArEventLogCreate_0.Execute = 0;
				Handler->LogIdent = Handler->ArEventLogCreate_0.Ident;				
				ArEventLogCreate(&Handler->ArEventLogCreate_0);
				if (Handler->LogFifo.PendingEntries > 0)
				{	  
					writeLog = 1;					
					Status = ERR_FUB_BUSY;
				}
				else
				{
					Status = ERR_OK;
				}
			} 
				//Error case
			else if (Handler->ArEventLogCreate_0.Error) 
			{
				Handler->ArEventLogCreate_0.Execute = 0;
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogCreate_0.StatusID;		
				Status = Handler->ArEventLogCreate_0.StatusID;							
				ArEventLogCreate(&Handler->ArEventLogCreate_0);
			}
		}

		
		//Check if it is necessary to write the log
		if (writeLog && Handler->LogFifo.PendingEntries > 0)
		{					
			
			Status = ERR_FUB_BUSY;			
			Handler->ArEventLogWrite_0.Execute = 1;
			Handler->ArEventLogWrite_0.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
			Handler->ArEventLogWrite_0.AddDataSize = brsstrlen((UDINT)&Handler->LogEntry.AsciiData) + 1;
			Handler->ArEventLogWrite_0.AddData = (UDINT)&Handler->LogEntry.AsciiData;
			Handler->ArEventLogWrite_0.EventID = ArEventLogMakeEventID(Handler->LogEntry.LogLevel,0,Handler->LogEntry.ErrorNumber);
			brsstrcpy((UDINT)&Handler->ArEventLogWrite_0.ObjectID, (UDINT)&Handler->LoggerName);
			Handler->ArEventLogWrite_0.Ident = Handler->LogIdent;
			ArEventLogWrite(&Handler->ArEventLogWrite_0);
			
			//Written OK. Quit from the queue and finish the process
			if (Handler->ArEventLogWrite_0.Done) 
			{
				Handler->ArEventLogWrite_0.Execute = 0;
				ArEventLogWrite(&Handler->ArEventLogWrite_0);
				
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
			else if (Handler->ArEventLogWrite_0.Error) 
			{
				Handler->ArEventLogWrite_0.Execute = 0;
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogWrite_0.StatusID;
				Status = Handler->ArEventLogWrite_0.StatusID;				
				ArEventLogWrite(&Handler->ArEventLogWrite_0);				
			}
		}			
	}
	
	Handler->ErrorInternal = Status;
	
	return Status;
}



/* Function to insert messages in custom loggers in an aeasy way. The Executed is detected by Edge*/
long int EasyLog(plcbit Execute, struct InputsEasyLogTyp* LoggerInfo)
{
	//Initialize the different status
	long int Status = ERR_OK;	
	long int StatusEnqueue = ERR_OK;
	long int StatusWrite = ERR_OK;
		
	//Get the pointer to the logger name
	char* pCharMessage = LoggerInfo->Message;
	unsigned long pMessage = reinterpret_cast<unsigned long>(pCharMessage);
	
	//Get the pointer to the message
	char* pCharLoggerName = LoggerInfo->LoggerName;
	unsigned long pLoggerName = reinterpret_cast<unsigned long>(pCharLoggerName);
		
	//Search the map to check the last execute
	struct LoggerHandler_type* Handler = SearchMap(pLoggerName);
	
	//Command to insert a new message
	if (Execute)
	{
		//Check that the parameters are correct when an insertion is needed
		if (strcmp(LoggerInfo->Message, "") == 0 || strcmp(LoggerInfo->LoggerName, "") == 0 || LoggerInfo->LogLevel < 0 || LoggerInfo->LogLevel > 3)
		{
			Status = ERR_BUR_ILLPAR;
		}
		else
		{
			//Enqueue the message in the Fifo
			StatusEnqueue = UserLogEnqueue( LoggerInfo->LogLevel, LoggerInfo->ErrorNumber, pMessage, pLoggerName, Handler);			
		}
	}

	//Write the pending messages (or create the logger in the first insertion)
  	StatusWrite = UserLogWrite(Handler);
	
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


