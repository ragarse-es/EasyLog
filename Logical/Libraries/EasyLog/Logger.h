#ifndef LOGGER_H
#define LOGGER_H

#include <stdlib.h>
#include "AsBrStr.h"
#include <cstring>
#include <string>

extern "C" 
{
  long int EasyLog(plcbit Execute, plcstring* LoggerName, unsigned char LogLevel, unsigned short ErrorNumber, plcstring* Message);
}

#endif // LOGGER_H



