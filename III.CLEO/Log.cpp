#include "Log.h"
#include <Windows.h>
#include <stdio.h>

char Log::fileName[512];
FILE *Log::logFile;
bool Log::opened = false;
unsigned int Log::logPriority = LOG_PRIORITY_DEFAULT;

void Log::Initialise(char *filepath)
{
	strcpy(fileName, filepath);
	logFile = fopen(filepath, "wt");
	opened = true;
}

void Log::Restart()
{
	if(opened)
	{
		fclose(logFile);
		logFile = fopen(fileName, "at");
	}
}

void Log::Close()
{
	if(opened)
	{
		fclose(logFile);
		opened = false;
	}
}

void Log::Record(unsigned int priority, char *format, ...)
{
	if(priority > logPriority || !opened)
		return;
	char final[512];
	va_list ap;
	va_start(ap, format);
	vsnprintf(final, 512, format, ap);
	va_end(ap);
	fputs(final, logFile);
}

void Log::RecordAndRestart(unsigned int priority, char *format, ...)
{
	if(priority > logPriority || !opened)
		return;
	char final[512];
	va_list ap;
	va_start(ap, format);
	vsnprintf(final, 512, format, ap);
	va_end(ap);
	fputs(final, logFile);
	Restart();
}

void Log::RecordLine(unsigned int priority, char *format, ...)
{
	if(priority > logPriority || !opened)
		return;
	char final[512];
	va_list ap;
	va_start(ap, format);
	vsnprintf(final, 512, format, ap);
	va_end(ap);
	fputs(final, logFile);
	fputs("\n", logFile);
}

void Log::RecordLineAndRestart(unsigned int priority, char *format, ...)
{
	if(priority > logPriority || !opened)
		return;
	char final[512];
	va_list ap;
	va_start(ap, format);
	vsnprintf(final, 512, format, ap);
	va_end(ap);
	fputs(final, logFile);
	fputs("\n", logFile);
	Restart();
}