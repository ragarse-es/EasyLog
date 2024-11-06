
{REDUND_OK} FUNCTION UserLogWrite : DINT (*Function to execute asyncronously with the Logger name to insert the messages in the logger that has been inserted with the UserLogEnqueue function. If returned vale 0 it is OK. Any other value search the error in the AS Help*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		pLoggerName : UDINT; (*Pointer to the name of the Logger to write*) (* *) (*#PAR*)
	END_VAR
END_FUNCTION

FUNCTION UserLogEnqueue : DINT (*Function to execute when is necessary a log insertion. It will be enqueued in a Fifo until the UserLogWrite inserts the messages in the logger. If returned vale 0 it is OK. Any other valuesearch the error in the AS Help*)
	VAR_INPUT
		LogLevel : USINT; (*Severity of the error. Coud be Success (0), Info (1), Warning (2) or Error (3)*) (* *) (*#PAR*)
		ErrorNumber : UINT; (*Number of the error or information to insert*) (* *) (*#PAR*)
		pAsciiData : UDINT; (*Pointer to the string that specify the message to insert in the logger*) (* *) (*#PAR*)
		pLoggerName : UDINT; (*Pointer to the string with the name of the Logger*) (* *) (*#PAR*)
	END_VAR
END_FUNCTION
