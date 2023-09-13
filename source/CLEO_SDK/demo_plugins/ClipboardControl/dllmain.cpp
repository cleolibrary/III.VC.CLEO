#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if CLEO_VC
#pragma comment (lib, "VC.CLEO.lib")
#include "VC.CLEO.h"
#else
#pragma comment (lib, "III.CLEO.lib")
#include "III.CLEO.h"
#endif

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   1
#define CLEO_VERSION_MINOR   0

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 24)|(CLEO_VERSION_MAJOR << 16)|(CLEO_VERSION_MINOR << 8))

tScriptVar *Params;

eOpcodeResult WINAPI OPCODE_0B20(CScript *script)
/****************************************************************
Opcode Format
0B20=2,read_clipboard_data %1d% size %2d%
****************************************************************/
{
	script->Collect(2);
	HANDLE ClipbData;
	if (OpenClipboard(0))
	{
		ClipbData = GetClipboardData(1u);
		if (ClipbData)
		{
			memcpy(Params[0].pVar, ClipbData, Params[1].nVar);
			script->UpdateCompareFlag(true);
		}
		CloseClipboard();
	}
	return OR_CONTINUE;
}

eOpcodeResult WINAPI OPCODE_0B21(CScript *script)
/****************************************************************
Opcode Format
0B21=2,write_clipboard_data %1d% size %2d%
****************************************************************/
{
	script->Collect(2);

	HGLOBAL hGl, hMem;
	void *Lock;

	if (OpenClipboard(0))
	{
		EmptyClipboard();
		hGl = GlobalAlloc(0x2042u, 0x800u);
		hMem = hGl;
		if (hGl)
		{
			Lock = GlobalLock(hGl);
			memcpy(Lock, Params[0].pVar, Params[1].nVar);
			GlobalUnlock(hMem);
			SetClipboardData(1u, hMem);
			script->UpdateCompareFlag(true);
		}
		CloseClipboard();
	}
	return OR_CONTINUE;
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "ClipboardControl.cleo", MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
		Opcodes::RegisterOpcode(0x0B20, OPCODE_0B20);
		Opcodes::RegisterOpcode(0x0B21, OPCODE_0B21);
	}
	return TRUE;
}