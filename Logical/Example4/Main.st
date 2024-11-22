

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
		LoggerName2 := CONCAT(LoggerName2, '2');
		brsmemcpy(ADR(LoggerName3), ADR(LoggerName), SIZEOF(LoggerName));	  
		LoggerName3 := CONCAT(LoggerName3, '3');
		brsmemcpy(ADR(LoggerName4), ADR(LoggerName), SIZEOF(LoggerName));	  
		LoggerName4 := CONCAT(LoggerName4, '4');

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
		
		IF BothInserts THEN
			InsertMessages := TRUE;
			InsertMessages2 := TRUE;
			InsertMessages3 := TRUE;
			InsertMessages4 := TRUE;
		END_IF;
		
		IF InsertMessages THEN	 
			InsertMessages := FALSE;
			Status := EasyLog(InsertMessages, LoggerName, arEVENTLOG_SEVERITY_INFO, 123,  Message);
		END_IF;
		IF InsertMessages2 THEN	 
			InsertMessages2 := FALSE;
			Status := EasyLog(InsertMessages2, LoggerName2, arEVENTLOG_SEVERITY_INFO, 123,  Message);		
		END_IF;	
		
		BothInserts := FALSE;
		
//		IF InsertMessages3 THEN	  
//			Status := EasyLog(InsertMessages3, LoggerName3, arEVENTLOG_SEVERITY_INFO, 123,  Message);		
//		END_IF;
//		IF InsertMessages4 THEN	  
//			Status := EasyLog(InsertMessages4, LoggerName4, arEVENTLOG_SEVERITY_INFO, 123,  Message);		
//		END_IF;	
		
	//	Status := EasyLog(InsertMessages2, LoggerName2, arEVENTLOG_SEVERITY_INFO, 123,  Message);		
		

	
		IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
			//Check the error and solve it
		END_IF;
	END_FOR;
	
	//	InsertMessages := FALSE;


	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

