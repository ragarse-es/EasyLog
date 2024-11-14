
PROGRAM _INIT
	
END_PROGRAM

PROGRAM _CYCLIC
		
	
	// Convert the current temperature into a string
	itoa(Temperature, ADR(TempStr));
	TempStr := CONCAT(TempStr, ' ºC');
 
	// Check if it is greater than the Alarm Limit and create the message
	IF Temperature > AlarmLimit THEN
		InfoLog1.Message := CONCAT('Current Temperature: ', TempStr);
		InfoLog1.Message := CONCAT(InfoLog1.Message, ' above the Alarm limit: ');
		itoa(AlarmLimit, ADR(AlarmLimitStr));
		AlarmLimitStr := CONCAT(AlarmLimitStr, ' ºC');
		InfoLog1.Message := CONCAT(InfoLog1.Message, AlarmLimitStr);
		TemperatureAlarm := TRUE;
	ELSE;
		TemperatureAlarm := FALSE;
	END_IF;
 
	// Check if it is greater than the Warning Limit and create the message
	IF Temperature > WarningLimit THEN
		InfoLog2.Message := CONCAT('Current Temperature: ', TempStr);
		InfoLog2.Message := CONCAT(InfoLog2.Message, ' above the Warning limit: ');
		itoa(WarningLimit, ADR(WarningLimitStr));
		WarningLimitStr := CONCAT(WarningLimitStr, ' ºC');
		InfoLog2.Message := CONCAT(InfoLog2.Message, WarningLimitStr);
		TemperatureWarning := TRUE;
	ELSE;
		TemperatureWarning := FALSE;
	END_IF;
 
	//Make the calls to the functions with EDGEPOS to make only 1 insertion when the alarm raise
	Status1 := EasyLog(EDGEPOS(TemperatureAlarm), InfoLog1);
	Status2 := EasyLog(EDGEPOS(TemperatureWarning), InfoLog2);
 
	IF Status1 <> ERR_OK AND Status1 <> ERR_FUB_BUSY THEN
		//Check the error and solve it
	END_IF;
 
	IF Status2 <> ERR_OK AND Status2 <> ERR_FUB_BUSY THEN
		//Check the error and solve it
	END_IF;

	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

