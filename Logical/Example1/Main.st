(*********************************************************************************
 * Copyright: B&R Industrial Automation GmbH 
*********************************************************************************
 * Program: Example3 
 * File: Example 3 - EasyLogLibrary 
 * Author:    B&R Spain
 * Created:   November 18, 2024/10:04 AM 
*********************************************************************************
 * Description: Example to show how to insert cyclicaly the value of a variable
	in a logger
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
	InfoLog.LoggerName := 'Temp';
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
	InfoLog.Message := CONCAT('Current Temperature: ', TempStr);
	
	// Execute the logger insertion if InsertMessages = TRUE
	Status := EasyLog(InsertMessages, InfoLog);
 
	IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
		//Check the error in the help and solve it
	END_IF;
 	
	
END_PROGRAM

PROGRAM _EXIT
	 
END_PROGRAM

