#pragma once
#include <stdio.h>

#define LOG_PRIORITY_LEVEL0 0
#define LOG_PRIORITY_ALWAYS LOG_PRIORITY_LEVEL0
#define LOG_PRIORITY_LEVEL1 1
#define LOG_PRIORITY_LEVEL2 2
#define LOG_PRIORITY_LEVEL3 3
#define LOG_PRIORITY_LEVEL4 4
#define LOG_PRIORITY_SCRIPT_LOADING LOG_PRIORITY_LEVEL1
#define LOG_PRIORITY_OPCODE_ID LOG_PRIORITY_LEVEL3
#define LOG_PRIORITY_OPCODE_PARAMS LOG_PRIORITY_LEVEL4
#define LOG_PRIORITY_MEMORY_ALLOCATION LOG_PRIORITY_LEVEL2
#define LOG_PRIORITY_GAME_EVENT LOG_PRIORITY_ALWAYS
#define LOG_PRIORITY_CUSTOM_TEXT LOG_PRIORITY_LEVEL2
#define LOG_PRIORITY_OPCODE LOG_PRIORITY_LEVEL2
#define LOG_PRIORITY_REGISTER_OPCODE LOG_PRIORITY_LEVEL1

#define LOG_PRIORITY_DEFAULT LOG_PRIORITY_LEVEL2

class Log
{
	static FILE *logFile;

	static char fileName[512];

	static bool opened;

	static unsigned int logPriority;

public:
	static void Initialise(char *filepath);

	static void Close();

	static void Restart();

	static void Record(unsigned int priority, char *string, ...);

	static void RecordAndRestart(unsigned int priority, char *string, ...);

	static void RecordLineAndRestart(unsigned int priority, char *string, ...);

	static void RecordLine(unsigned int priority, char *string, ...);
};

#define LOG Log::Record
#define LOGL Log::RecordLine
#define LOGQ Log::RecordAndRestart
#define LOGLQ Log::RecordLineAndRestart