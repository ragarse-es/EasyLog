
TYPE
	InputsEasyLogTyp : 	STRUCT 
		LogLevel : USINT; (*Severity of the error. Coud be Success (0), Info (1), Warning (2) or Error (3)*)
		ErrorNumber : UINT; (*Number of the error or information to insert*)
		pMessage : UDINT; (*Pointer to the string that specify the message to insert in the logger*)
		pLoggerName : UDINT; (*Pointer to the string with the name of the Logger*)
	END_STRUCT;
END_TYPE
