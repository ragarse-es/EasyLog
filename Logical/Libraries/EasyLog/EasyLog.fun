
FUNCTION EasyLog : DINT (*Status returned by the function. Status <> ERR_OK should be checked*)
	VAR_INPUT
		Execute : BOOL; (*Input to add a message in the logger. Every cycle with TRUE in this input will insert a logger message. *) (* *) (*#PAR*)
		LoggerName : STRING[10]; (*String with the name of the Logger. Max Lenght 10 characters.*) (* *) (*#PAR*)
		LogLevel : USINT; (*Severity of the error. Coud be arEVENTLOG_SEVERITY_SUCCESS, arEVENTLOG_SEVERITY_INFO,  arEVENTLOG_SEVERITY_WARNING or arEVENTLOG_SEVERITY_ERROR*) (* *) (*#PAR*)
		ErrorNumber : UINT; (*Number of the error or information to insert*) (* *) (*#PAR*)
		Message : STRING[256]; (*String that specify the message to insert in the logger. Max lenght 256 characters*) (* *) (*#PAR*)
	END_VAR
END_FUNCTION
