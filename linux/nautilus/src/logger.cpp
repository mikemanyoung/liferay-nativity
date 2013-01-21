#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef ENABLE_LOG

void writeLog(const char* format, ...)
{
	va_list args;
	FILE* file = NULL;
	va_start (args, format);
	
	file = fopen("/home/user/LiferayPlugin.log","a+");	
	
	if (file)
	{
		vfprintf (file, format, args);
		fclose(file);
	}
	
	va_end (args); 
}


#else

void writeLog(const char* format, ...)
{
}

#endif
