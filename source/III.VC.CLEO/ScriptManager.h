#pragma once

#include "CustomScript.h"
#include <list>

class ScriptManager
{
public:
	CScript gameScripts[128];

	std::list<char *> scriptMemory;

	CScript *pCusomScripts;

	unsigned int numLoadedCustomScripts;

	ScriptManager();

	char *AllocateMemoryForScript(char *scriptName, unsigned int size);

	void DeleteScriptMemory(char *scriptName, char *data);

	void ReleaseScriptsMemory();

	void LoadScripts();

	void UnloadScripts();

	void DisableAllScripts();

	void EnableAllScripts();

	static void __fastcall InitialiseScript(CScript *script);

	static eOpcodeResult __fastcall ProcessScriptCommand(CScript *script);

	static void __fastcall CollectScriptParameters(CScript *script, int, unsigned int *pIp, unsigned int numParams);

	static int __fastcall CollectScriptNextParameterWithoutIncreasingPC(CScript *script, int, unsigned int ip);
};

extern ScriptManager scriptMgr;