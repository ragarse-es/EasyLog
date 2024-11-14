
FUNCTION EasyLog : DINT (*Status returned by the function. Status <> ERR_OK should be checked*)
	VAR_INPUT
		Execute : BOOL; (*Input to add a message in the logger. Every cycle with TRUE in this input will insert a logger message. Recommended to use EDGEPOS()*) (* *) (*#PAR*)
		LogInfo : InputsEasyLogTyp; (*Input with all the information to insert the message in the logger. Severity, information, number and logger name*) (* *) (*#PAR*)
	END_VAR
END_FUNCTION
