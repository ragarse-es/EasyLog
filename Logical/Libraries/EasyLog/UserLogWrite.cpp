
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
#include <iostream>
#include <cstring>
#include <string>
#include <map>

#define sdRB_ERR_ASSERT 1001

/**
 * Internal Data Map
 *
 * This map is used to store and retrieve the internal state for each instance, allowing
 * for the management of multiple instances within the system.
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
	
//	std::strncpy(loggerName, (char*)inst, sizeof(loggerName) - 1);
	
	std::map<std::string, struct LoggerHandler_type>::iterator it;
	it = internalMap.find(loggerName);
	if (it == internalMap.end())
	{
		struct LoggerHandler_type newHandler = {0};
		
		newHandler.LoggerName[11] = {0};
		
		std::strncpy(newHandler.LoggerName, loggerName.c_str(), sizeof(newHandler.LoggerName) - 1);
		
		//		brsstrcpy((unsigned long)*newHandler.LoggerName, inst);
		internalMap[loggerName] = newHandler;
		return &internalMap[loggerName];	
	} else
	{
		return &it->second;
	}
}



/* Function to execute when is necessary a log insertion. 
	It will be enqueued in a Fifo until the UserLogWrite inserts the messages in the logger*/
long int UserLogEnqueue(unsigned char LogLevel, unsigned short ErrorNumber, unsigned long pAsciiData, unsigned long pLoggerName)
{
	
	long int Status = ERR_OK;
	
	//Search the Handler
	struct LoggerHandler_type* Handler = SearchMap(pLoggerName);
	
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
long int UserLogWrite(unsigned long pLoggerName)
{
	//Initialize the values
	long int Status = ERR_OK;
	
	struct LoggerHandler_type* Handler = SearchMap(pLoggerName);
	
	char *localLoggerName = (char *)pLoggerName;
	
	// Local temporary variables which are used in many places
	UDINT i;
	
	//State machine for the Log Step
	switch (Handler->LogStep)
	{
		case LOGGER_WAIT:
			Handler->ErrorInternal = ERR_OK;
			
			if (Handler->LogFifo.PendingEntries > 0) 
			{
				Handler->LogEntry = Handler->LogFifo.Entry[0];
				for (i = 0; i < Handler->LogFifo.PendingEntries - 1; i++) 
				{
					Handler->LogFifo.Entry[i] = Handler->LogFifo.Entry[i+1];
				}
				brsmemset((UDINT)&Handler->LogFifo.Entry[Handler->LogFifo.PendingEntries-1], 0 , sizeof(Handler->LogFifo.Entry[0]));
				Handler->LogFifo.PendingEntries--;
				Handler->LogStep = LOGGER_GET_INFO_LOG;
			}
			break;
	            
	            
		case LOGGER_GET_INFO_LOG:
			Handler->ArEventLogGetIdent_0.Execute = 1;
			brsstrcpy((UDINT)&Handler->ArEventLogGetIdent_0.Name, (UDINT)&Handler->LoggerName);   
	           
			if (Handler->ArEventLogGetIdent_0.Done) 
			{
				Handler->ArEventLogGetIdent_0.Execute = 0;
				Handler->LogIdent = Handler->ArEventLogGetIdent_0.Ident;
				Handler->LogStep = LOGGER_WRITE;
			} 
			else if (Handler->ArEventLogGetIdent_0.StatusID == arEVENTLOG_ERR_LOGBOOK_NOT_FOUND) 
			{
				Handler->ArEventLogGetIdent_0.Execute = 0;
				Handler->LogStep = LOGGER_CREATE_LOG;
				Status = arEVENTLOG_ERR_LOGBOOK_NOT_FOUND;
			} 
			else if (Handler->ArEventLogGetIdent_0.Error) 
			{
				Handler->ArEventLogGetIdent_0.Execute = 0;
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogGetIdent_0.StatusID;
				Handler->LogStep = LOGGER_WAIT;
				Status = Handler->ArEventLogGetIdent_0.StatusID;
			}
			break;
	           
	           
		case LOGGER_CREATE_LOG:
			Handler->ArEventLogCreate_0.Execute = 1;
			brsstrcpy((UDINT)&Handler->ArEventLogCreate_0.Name, (UDINT)&Handler->LoggerName);
			Handler->ArEventLogCreate_0.Size = 200000;
			Handler->ArEventLogCreate_0.Persistence = arEVENTLOG_PERSISTENCE_PERSIST;
	                     
			if (Handler->ArEventLogCreate_0.Done) 
			{
				Handler->ArEventLogCreate_0.Execute = 0;
				Handler->LogIdent = Handler->ArEventLogCreate_0.Ident;
				Handler->LogStep = LOGGER_WRITE;
			} 
			else if (Handler->ArEventLogCreate_0.Error) 
			{
				Handler->ArEventLogCreate_0.Execute = 0;
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogCreate_0.StatusID;
				Handler->LogStep = LOGGER_WAIT;
				Status = Handler->ArEventLogCreate_0.StatusID;
			}
			break;
	           
	           
		case LOGGER_WRITE:
			Handler->ArEventLogWrite_0.Execute = 1;
			Handler->ArEventLogWrite_0.AddDataFormat = arEVENTLOG_ADDFORMAT_TEXT;
			Handler->ArEventLogWrite_0.AddDataSize = brsstrlen((UDINT)&Handler->LogEntry.AsciiData) + 1;
			Handler->ArEventLogWrite_0.AddData = (UDINT)&Handler->LogEntry.AsciiData;
			Handler->ArEventLogWrite_0.EventID = ArEventLogMakeEventID(Handler->LogEntry.LogLevel,0,Handler->LogEntry.ErrorNumber);
			brsstrcpy((UDINT)&Handler->ArEventLogWrite_0.ObjectID, (UDINT)&Handler->LoggerName);
			Handler->ArEventLogWrite_0.Ident = Handler->LogIdent;
	                       
			if (Handler->ArEventLogWrite_0.Done) 
			{
				Handler->ArEventLogWrite_0.Execute = 0;
				Handler->LogStep = LOGGER_WAIT;
			} 
			else if (Handler->ArEventLogWrite_0.Error) 
			{
				Handler->ArEventLogWrite_0.Execute = 0;
				Handler->ErrorInternal = Handler->ErrorInternal ? Handler->ErrorInternal : Handler->ArEventLogWrite_0.StatusID;
				Handler->LogStep = LOGGER_WAIT;
				Status = Handler->ArEventLogWrite_0.StatusID;
				
			}
			break;
	           
		default:
			Handler->LogStep = LOGGER_WAIT;
			break;
	           
	}
	// Call ArEventLog FBs 
	ArEventLogGetIdent(&Handler->ArEventLogGetIdent_0);
	ArEventLogCreate(&Handler->ArEventLogCreate_0);
	ArEventLogWrite(&Handler->ArEventLogWrite_0);
	
	return Status;
}


/* Function to insert messages in custom loggers in an aeasy way */
long int EasyLog(plcbit Execute, struct InputsEasyLogTyp* LoggerInfo)
{
	
}


