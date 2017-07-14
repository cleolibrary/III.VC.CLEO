#pragma once
#include "CustomScript.h"
#include "Error.h"
#include <Windows.h>
 
#define CUSTOM_OPCODE_START_ID 0x05DC
#define MAX_NUMBER_OF_OPCODES 0x8000

typedef eOpcodeResult (__stdcall* Opcode)(CScript *);
typedef eOpcodeResult (__thiscall* OpcodeHandler)(CScript *, int);

class Opcodes
{
public:
	static Opcode functions[MAX_NUMBER_OF_OPCODES];

	static bool CLEOAPI RegisterOpcode(unsigned short id, Opcode func);
};