#pragma once

#include "CustomScript.h"

class ScmFunction
{
public:
	class ScmFunction *prev;
	unsigned int vars[16];
	int retAddr;

	ScmFunction(class CScript *script);
};