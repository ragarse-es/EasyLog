
PROGRAM _INIT
	(* Set the parameters of the logger *)
	InfoLog1.ErrorNumber := 123;
	InfoLog1.LogLevel := 1;
	InfoLog1.LoggerName := 'Temp';
	InfoLog1.Message := 'TestMessage';
		
	//This is only necessary if Cycle time < 50ms
	//Repeat until finish the process, with or without errors
	REPEAT
	  Status1 := EasyLog(FALSE, InfoLog1);
		UNTIL Status1 <> ERR_FUB_BUSY 
	END_REPEAT;
		
	
END_PROGRAM

PROGRAM _CYCLIC
	
    // Convert the current temperature into a string
	itoa(Temperature, ADR(TempStr));
	TempStr := CONCAT(TempStr, ' ºC');
	InfoLog1.Message := CONCAT('Current Temperature: ', TempStr);
	
	// Execute the logger insertion if InsertMessages = TRUE
	Status1 := EasyLog(InsertMessages, InfoLog1);
 
	IF Status1 <> ERR_OK AND Status1 <> ERR_FUB_BUSY THEN
		//Check the error in the help and solve it
	END_IF;
 	
	
END_PROGRAM

PROGRAM _EXIT
	 
END_PROGRAM

