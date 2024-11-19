

PROGRAM _INIT
	(* Set an initial message *)
	Message := 'TestMessage';
	
		
	//Repeat until finish the process, with or without errors
	// This will create the logger if it does not exist
//	REPEAT
//		Status := EasyLog(FALSE, LoggerName, arEVENTLOG_SEVERITY_INFO, 123,  Message);
//		UNTIL Status <> ERR_FUB_BUSY 
//	END_REPEAT;
		
END_PROGRAM

PROGRAM _CYCLIC
	
	IF brsmemcmp(ADR(OldLoggerName), ADR(LoggerName), SIZEOF(LoggerName)) <> 0 THEN
		MessageIndex := 0;
		brsmemcpy(ADR(OldLoggerName), ADR(LoggerName), SIZEOF(LoggerName));
		brsmemcpy(ADR(LoggerName2), ADR(LoggerName), SIZEOF(LoggerName));	  
		LoggerName2 := CONCAT(LoggerName2, '_2');

	END_IF; 
	
	
	FOR i := 1 TO MessagesPerCycle DO
		//Message index inserted in this logger
		brsitoa(MessageIndex, ADR(AuxStr));	
		MessageIndex := MessageIndex + 1;
		
		Message := CONCAT('Index = ', AuxStr);	
		brsitoa(i, ADR(AuxStr));
		//Creation of the message
		Message := CONCAT(Message,' .Message number ');
		Message := CONCAT(Message, AuxStr);		
		Message := CONCAT(Message, ' from   ');		  
		brsitoa(MessagesPerCycle, ADR(AuxStr));
		Message := CONCAT(Message, AuxStr);			
		Message := CONCAT(Message, ' inserted this cycle ');
		//Register the change
			
		
		Status := EasyLog(InsertMessages, LoggerName, arEVENTLOG_SEVERITY_INFO, 123,  Message);
		IF InsertMessages2 THEN	  
			Status := EasyLog(InsertMessages2, LoggerName2, arEVENTLOG_SEVERITY_INFO, 123,  Message);		
		END_IF;

	
		IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
			//Check the error and solve it
		END_IF;
	END_FOR;
	
	//	InsertMessages := FALSE;


	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

