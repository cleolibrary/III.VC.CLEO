#pragma once

#include <wtypes.h>

enum eLogicalOperation : WORD;

class ScmFunction
{
public:
	class ScmFunction* prev;
	unsigned int vars[16];
	int retAddr;

	BYTE* savedStack[6]; // gosub stack
	WORD savedSP;

	bool savedCondResult;
	eLogicalOperation savedLogicalOp;
	bool savedNotFlag;

	ScmFunction(class CScript *script);
	static void Return(class CScript* script);
};
