
TYPE
	InputsEasyLogTyp : 	STRUCT 
		LogLevel : USINT; (*Severity of the error. Coud be arEVENTLOG_SEVERITY_SUCCESS, arEVENTLOG_SEVERITY_INFO,  arEVENTLOG_SEVERITY_WARNING or arEVENTLOG_SEVERITY_ERROR*)
		ErrorNumber : UINT; (*Number of the error or information to insert*)
		Message : STRING[256]; (*String that specify the message to insert in the logger. Max lenght 256 characters*)
		LoggerName : STRING[10]; (*String with the name of the Logger. Max Lenght 10 characters*)
	END_STRUCT;
END_TYPE
