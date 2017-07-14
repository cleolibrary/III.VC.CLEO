#include "Error.h"
#include <stdio.h>
#include <Windows.h>

bool Error(char *message, ...)
{
	char text[512];
	va_list ap;
	va_start(ap, message);
	vsnprintf(text, 512, message, ap);
	va_end(ap);
	MessageBoxA(NULL, text, "CLEO.asi", 0);
	return false;
}