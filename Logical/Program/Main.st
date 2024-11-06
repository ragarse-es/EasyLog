
PROGRAM _INIT
	(* Insert code here *)
	 
END_PROGRAM

PROGRAM _CYCLIC
	

	IF InsertLog THEN
		InsertLog := FALSE;		
//		Objective, change the FB TO pass the Logger name instead OF the LogFifo
		UserLogEnqueue(arEVENTLOG_SEVERITY_ERROR, ErrorNumberTest,ADR(LogMessageTest),ADR(LoggerName));
		UserLogEnqueue(arEVENTLOG_SEVERITY_ERROR, ErrorNumberTest,ADR(LogMessageTest2),ADR(LoggerName2));
		
	END_IF;
	
	  
		Status1 := UserLogWrite(ADR(LoggerName));
		Status2 := UserLogWrite(ADR(LoggerName2));

	
	
	
END_PROGRAM

PROGRAM _EXIT
	(* Insert code here *)
	 
END_PROGRAM

