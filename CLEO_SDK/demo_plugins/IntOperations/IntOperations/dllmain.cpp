#include "stdafx.h"
#if _VC
#pragma comment (lib, "..\\..\\..\\..\\Release\\VC.CLEO.lib")
#include "..\..\..\VC.CLEO.h"
#else if _III
#pragma comment (lib, "..\\..\\..\\..\\Release\\III.CLEO.lib")
#include "..\..\..\III.CLEO.h"
#endif

#define OPCODE_INTOP_AND			0x0B10
#define OPCODE_INTOP_OR				0x0B11
#define OPCODE_INTOP_XOR			0x0B12
#define OPCODE_INTOP_NOT			0x0B13
#define OPCODE_INTOP_MOD			0x0B14
#define OPCODE_INTOP_SHR			0x0B15
#define OPCODE_INTOP_SHL			0x0B16
#define SCR_INTOP_AND		 		0x0B17
#define SCR_INTOP_OR				0x0B18
#define SCR_INTOP_XOR				0x0B19
#define SCR_INTOP_NOT				0x0B1A
#define SCR_INTOP_MOD				0x0B1B
#define SCR_INTOP_SHR				0x0B1C
#define SCR_INTOP_SHL				0x0B1D

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  0

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

tScriptVar *Params;

eOpcodeResult WINAPI Script_IntOp_AND(CScript *script)
/****************************************************************
Opcode Format
0B10=3,%3d% = %1d% AND %2d%
****************************************************************/
{
	script->Collect(2);
	Params[0].nVar = Params[0].nVar & Params[1].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_OR(CScript *script)
/****************************************************************
Opcode Format
0B11=3,%3d% = %1d% OR %2d%
****************************************************************/
{
	script->Collect(2);
	Params[0].nVar = Params[0].nVar | Params[1].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_XOR(CScript *script)
/****************************************************************
Opcode Format
0B12=3,%3d% = %1d% XOR %2d%
****************************************************************/
{
	script->Collect(2);
	Params[0].nVar = Params[0].nVar ^ Params[1].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_NOT(CScript *script)
/****************************************************************
Opcode Format
0B13=2,%2d% = NOT %1d%
****************************************************************/
{
	script->Collect(1);
	Params[0].nVar = ~Params[0].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_MOD(CScript *script)
/****************************************************************
Opcode Format
0B14=3,%3d% = %1d% MOD %2d%
****************************************************************/
{
	script->Collect(2);
	Params[0].nVar = Params[0].nVar % Params[1].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_SHR(CScript *script)
/****************************************************************
Opcode Format
0B15=3,%3d% = %1d% SHR %2d%
****************************************************************/
{
	script->Collect(2);
	Params[0].nVar = Params[0].nVar >> Params[1].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_IntOp_SHL(CScript *script)
/****************************************************************
Opcode Format
0B16=3,%3d% = %1d% SHL %2d%
****************************************************************/
{
	script->Collect(2);
	Params[0].nVar = Params[0].nVar << Params[1].nVar;
	script->Store(1);
	return OR_CONTINUE;
}

/****************************************************************
Now do them as real operators...
*****************************************************************/

eOpcodeResult WINAPI Scr_IntOp_AND(CScript *script)
/****************************************************************
Opcode Format
0B17=2,%1d% &= %2d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	script->Collect(1);
	int val = Params[0].nVar;
	op->nVar &= val;
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_OR(CScript *script)
/****************************************************************
Opcode Format
0B18=2,%1d% |= %2d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	script->Collect(1);
	int val = Params[1].nVar;
	op->nVar |= val;
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_XOR(CScript *script)
/****************************************************************
Opcode Format
0B19=2,%1d% ^= %2d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	script->Collect(1);
	int val = Params[1].nVar;
	op->nVar ^= val;
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_NOT(CScript *script)
/****************************************************************
Opcode Format
0B1A=1,~%1d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	op->nVar = ~op->nVar;
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_MOD(CScript *script)
/****************************************************************
Opcode Format
0B1B=2,%1d% %= %2d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	script->Collect(1);
	int val = Params[1].nVar;
	op->nVar %= val;
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_SHR(CScript *script)
/****************************************************************
Opcode Format
0B1C=2,%1d% >>= %2d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	script->Collect(1);
	int val = Params[1].nVar;
	op->nVar >>= val;
	return OR_CONTINUE;
}

eOpcodeResult WINAPI Scr_IntOp_SHL(CScript *script)
/****************************************************************
Opcode Format
0B1D=2,%1d% <<= %2d%
****************************************************************/
{
	tScriptVar* op = (tScriptVar*)script->GetPointerToScriptVariable();
	script->Collect(1);
	int val = Params[1].nVar;
	op->nVar <<= val;
	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "IntOperations.cleo", MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
		Opcodes::RegisterOpcode(OPCODE_INTOP_AND, Script_IntOp_AND);
		Opcodes::RegisterOpcode(OPCODE_INTOP_OR, Script_IntOp_OR);
		Opcodes::RegisterOpcode(OPCODE_INTOP_XOR, Script_IntOp_XOR);
		Opcodes::RegisterOpcode(OPCODE_INTOP_NOT, Script_IntOp_NOT);
		Opcodes::RegisterOpcode(OPCODE_INTOP_MOD, Script_IntOp_MOD);
		Opcodes::RegisterOpcode(OPCODE_INTOP_SHR, Script_IntOp_SHR);
		Opcodes::RegisterOpcode(OPCODE_INTOP_SHL, Script_IntOp_SHL);
		Opcodes::RegisterOpcode(SCR_INTOP_AND, Scr_IntOp_AND);
		Opcodes::RegisterOpcode(SCR_INTOP_OR, Scr_IntOp_OR);
		Opcodes::RegisterOpcode(SCR_INTOP_XOR, Scr_IntOp_XOR);
		Opcodes::RegisterOpcode(SCR_INTOP_NOT, Scr_IntOp_NOT);
		Opcodes::RegisterOpcode(SCR_INTOP_MOD, Scr_IntOp_MOD);
		Opcodes::RegisterOpcode(SCR_INTOP_SHR, Scr_IntOp_SHR);
		Opcodes::RegisterOpcode(SCR_INTOP_SHL, Scr_IntOp_SHL);
	}
	return TRUE;
}