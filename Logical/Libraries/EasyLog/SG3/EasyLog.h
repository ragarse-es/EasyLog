/* Automation Studio generated header file */
/* Do not edit ! */
/* EasyLog 1.00.0 */

#ifndef _EASYLOG_
#define _EASYLOG_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _EasyLog_VERSION
#define _EasyLog_VERSION 1.00.0
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG3
		#include "AsBrStr.h"
		#include "sys_lib.h"
		#include "ArEventLog.h"
#endif
#ifdef _SG4
		#include "AsBrStr.h"
		#include "sys_lib.h"
		#include "ArEventLog.h"
#endif
#ifdef _SGC
		#include "AsBrStr.h"
		#include "sys_lib.h"
		#include "ArEventLog.h"
#endif


/* Constants */
#ifdef _REPLACE_CONST
 #define MAX_LOGGER_QUEUE 10U
#else
 _GLOBAL_CONST unsigned char MAX_LOGGER_QUEUE;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct InputsEasyLogTyp
{	unsigned char LogLevel;
	unsigned short ErrorNumber;
	plcstring Message[257];
	plcstring LoggerName[11];
} InputsEasyLogTyp;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC signed long EasyLog(plcbit Execute, struct InputsEasyLogTyp* LogInfo);


#ifdef __cplusplus
};
#endif
#endif /* _EASYLOG_ */

