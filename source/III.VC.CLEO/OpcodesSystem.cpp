#include "OpcodesSystem.h"
#include "Log.h"

Opcode Opcodes::functions[MAX_NUMBER_OF_OPCODES];

bool Opcodes::RegisterOpcode(unsigned short id, Opcode func)
{
	if(id >= MAX_NUMBER_OF_OPCODES)
	{
		LOGL(LOG_PRIORITY_REGISTER_OPCODE, "RegisterOpcode: Opcode ID is out of range (%04X > %04X)", id, MAX_NUMBER_OF_OPCODES - 1);
		return Error("RegisterOpcode: Opcode ID is out of range (%04X > %04X)", id, MAX_NUMBER_OF_OPCODES - 1);
	}
	if(functions[id])
		LOGL(LOG_PRIORITY_REGISTER_OPCODE, "RegisterOpcode: Opcode %04X was already registered", id);
	functions[id] = func;
	return true;
}