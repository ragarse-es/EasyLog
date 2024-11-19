

PROGRAM _INIT
	(* Set an initial message *)
	Message := 'TestMessage';
	
		
	//Repeat until finish the process, with or without errors
	// This will create the logger if it does not exist
	REPEAT
		Status := EasyLog(FALSE, 'TestLog', arEVENTLOG_SEVERITY_INFO, 123,  Message);
		UNTIL Status <> ERR_FUB_BUSY 
	END_REPEAT;
		
END_PROGRAM

PROGRAM _CYCLIC
	

	FOR i := 1 TO MessagesPerCycle DO	  
		brsitoa(i, ADR(AuxStr));
		//Creation of the message
		Message := CONCAT('Message number ', AuxStr);		
		Message := CONCAT(Message, ' from total of  ');		  
		brsitoa(MessagesPerCycle, ADR(AuxStr));
		Message := CONCAT(Message, AuxStr);			
		Message := CONCAT(Message, ' messages inserted this cycle ');
		
		//Register the change
		Status := EasyLog(InsertMessages, 'TestLog', arEVENTLOG_SEVERITY_INFO, 123,  Message);
	
		IF Status <> ERR_OK AND Status <> ERR_FUB_BUSY THEN
			//Check the error and solve it
		END_IF;
	END_FOR;
	
//	InsertMessages := FALSE;


	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

