(*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
*********************************************************************************
 * Program: Example2 
 * File: Example 2 - EasyLogLibrary 
 * Author:    B&R Spain
 * Created:   November 18, 2024/10:04 AM 
*********************************************************************************
 * Description: Example to show how to insert alarms in a logger
	-
 * Interface:
	-
*************************** VERSION CONTROL *************************************
   Version	| Date	                | Author	| Description
----------------------------------------------------------------------------------
   V1.00	| November 18, 2024		| B&R Spain	|  Initial version	
 *********************************************************************************)

PROGRAM _INIT
	(* Set the parameters of the logger *)
	InfoLog.ErrorNumber := 123;
	InfoLog.LogLevel := arEVENTLOG_SEVERITY_INFO;
	InfoLog.LoggerName := 'TempAlarms';
	InfoLog.Message := 'TestMessage';
		
	//Repeat until finish the process, with or without errors
	// This will create the logger if it does not exist
	REPEAT
		Status := EasyLog(FALSE, InfoLog);
		UNTIL Status <> ERR_FUB_BUSY 
	END_REPEAT;
		
END_PROGRAM

PROGRAM _CYCLIC
		
	
	// Convert the current temperature into a string
	brsitoa(Temperature, ADR(TempStr));
	TempStr := CONCAT(TempStr, ' ºC');
 
	(**********************************  Check the ALARM LIMIT  **********************************)
	// Check if it is greater than the Alarm Limit and create the message
	IF Temperature > AlarmLimit THEN
		InfoLog.LogLevel := arEVENTLOG_SEVERITY_ERROR;
		InfoLog.ErrorNumber := 1111; //Error number defined by the user		
		//Creation of the Message
		InfoLog.Message := CONCAT('Current Temperature: ', TempStr);
		InfoLog.Message := CONCAT(InfoLog.Message, ' above the Alarm limit: ');
		brsitoa(AlarmLimit, ADR(AlarmLimitStr));
		AlarmLimitStr := CONCAT(AlarmLimitStr, ' ºC');		
		InfoLog.Message := CONCAT(InfoLog.Message, AlarmLimitStr);
		
		//Boolean value of the Alarm
		TemperatureAlarm := TRUE;
	ELSE;
		TemperatureAlarm := FALSE;
	END_IF;
	
	//Make the calls to the functions with EDGEPOS to make only 1 insertion when the alarm raise
	Status := EasyLog(EDGEPOS(TemperatureAlarm), InfoLog);
	
	IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
		//Check the error and solve it
	END_IF;
 
	
	(**********************************  Check the WARNING LIMIT  **********************************)
	// Check if it is greater than the Warning Limit and create the message
	IF Temperature > WarningLimit THEN
		InfoLog.LogLevel := arEVENTLOG_SEVERITY_WARNING; 
		InfoLog.ErrorNumber := 2222; //Error number defined by the user		
		//Creation of the Message
		InfoLog.Message := CONCAT('Current Temperature: ', TempStr);
		InfoLog.Message := CONCAT(InfoLog.Message, ' above the Warning limit: ');
		brsitoa(WarningLimit, ADR(WarningLimitStr));
		WarningLimitStr := CONCAT(WarningLimitStr, ' ºC');
		InfoLog.Message := CONCAT(InfoLog.Message, WarningLimitStr);
		
		//Boolean value of the Alarm
		TemperatureWarning := TRUE;
	ELSE;
		TemperatureWarning := FALSE;
	END_IF;
 
	Status := EasyLog(EDGEPOS(TemperatureWarning), InfoLog); 
 
	IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
		//Check the error and solve it
	END_IF;

	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

