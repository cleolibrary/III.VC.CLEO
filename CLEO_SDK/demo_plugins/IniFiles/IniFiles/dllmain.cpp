#include "stdafx.h"
#include <stdio.h>

#if _VC
#pragma comment (lib, "..\\..\\..\\..\\Release\\VC.CLEO.lib")
#include "..\..\..\VC.CLEO.h"
#else if _III
#pragma comment (lib, "..\\..\\..\\..\\Release\\III.CLEO.lib")
#include "..\..\..\III.CLEO.h"
#endif

#define OPCODE_INIFILE_GET_INT		0x0AF0
#define OPCODE_INIFILE_WRITE_INT	0x0AF1  
#define OPCODE_INIFILE_GET_FLOAT	0x0AF2  
#define	OPCODE_INIFILE_WRITE_FLOAT	0x0AF3 
#define	OPCODE_INIFILE_READ_STRING	0x0AF4 
#define	OPCODE_INIFILE_WRITE_STRING	0x0AF5

#define CLEO_VERSION_MAIN    2
#define CLEO_VERSION_MAJOR   0
#define CLEO_VERSION_MINOR   0
#define CLEO_VERSION_BINARY  0

#define CLEO_VERSION ((CLEO_VERSION_MAIN << 16)|(CLEO_VERSION_MAJOR << 12)|(CLEO_VERSION_MINOR << 8)|(CLEO_VERSION_BINARY))

tScriptVar *Params;

char* MakeFullPath(char *path, char *dst)
{
	if (path[1] != ':')
	{
		//get current working directory
		GetCurrentDirectory(MAX_PATH, dst);
		strcat(dst, "\\");
		strcat(dst, path);
	}
	else
	{
		strcpy(dst, path);
	}
	return dst;
}

eOpcodeResult WINAPI Script_InifileGetInt(CScript* script)
/****************************************************************
Opcode Format
0AF0=4,%4d% = get_int_from_ini_file %1s% section %2s% key %3s%
****************************************************************/
{
	script->Collect(3);

	char iniPath[MAX_PATH];
	char path[100];
	char sectionName[100];
	char key[100];
	int result;

	strcpy(path, Params[0].cVar);
	strcpy(sectionName, Params[1].cVar);
	strcpy(key, Params[2].cVar);

	//if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
	MakeFullPath(path, iniPath);

	result = GetPrivateProfileInt(sectionName, key, 0x80000000, iniPath);
	Params[0].nVar = result;
	script->Store(1);
	script->UpdateCompareFlag(result != 0x80000000);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileWriteInt(CScript* script)
/****************************************************************
Opcode Format
0AF1=4,write_int %1d% to_ini_file %2s% section %3s% key %4s%
****************************************************************/
{
	script->Collect(4);

	char iniPath[MAX_PATH];
	char path[100];
	char sectionName[100];
	char key[100];
	DWORD value;
	char strValue[100];
	BOOL result;

	value = Params[0].nVar;
	strcpy(path, Params[1].cVar);
	strcpy(sectionName, Params[2].cVar);
	strcpy(key, Params[3].cVar);

	//if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
	MakeFullPath(path, iniPath);

	result = WritePrivateProfileString(sectionName, key, _itoa(value, strValue, 10), iniPath);
	script->UpdateCompareFlag(result != 0);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileGetFloat(CScript* script)
/****************************************************************
Opcode Format
0AF2=4,%4d% = get_float_from_ini_file %1s% section %2s% key %3s%
****************************************************************/
{
	script->Collect(3);

	char iniPath[MAX_PATH];
	char path[100];
	char sectionName[100];
	char key[100];
	float value = 0.0f;
	char strValue[100];
	BOOL result;

	strcpy(path, Params[0].cVar);
	strcpy(sectionName, Params[1].cVar);
	strcpy(key, Params[2].cVar);

	//if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
	MakeFullPath(path, iniPath);

	result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);
	if (result)
	{
		value = (float)atof(strValue);
		Params[0].fVar = value;
		script->Store(1);
	}
	else
	{
		Params[0].fVar = 0.0f;
		script->Store(1);
	}

	script->UpdateCompareFlag(result != 0);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileWriteFloat(CScript* script)
/****************************************************************
Opcode Format
0AF3=4,write_float %1d% to_ini_file %2s% section %3s% key %4s%
****************************************************************/
{
	script->Collect(4);

	char iniPath[MAX_PATH];
	char path[100];
	char sectionName[100];
	char key[100];
	float value;
	char strValue[100];
	BOOL result;

	value = Params[0].fVar;
	strcpy(path, Params[1].cVar);
	strcpy(sectionName, Params[2].cVar);
	strcpy(key, Params[3].cVar);

	//if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
	MakeFullPath(path, iniPath);

	sprintf(strValue, "%g", value);

	result = WritePrivateProfileString(sectionName, key, strValue, iniPath);
	script->UpdateCompareFlag(result != 0);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileReadString(CScript* script)
/****************************************************************
Opcode Format
0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
****************************************************************/
{
	script->Collect(3);

	char iniPath[MAX_PATH];
	char path[100];
	char sectionName[100];
	char key[100];
	char strValue[100];
	BOOL result;

	strcpy(path, Params[0].cVar);
	strcpy(sectionName, Params[1].cVar);
	strcpy(key, Params[2].cVar);

	//if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
	MakeFullPath(path, iniPath);

	result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);

	strcpy(Params[0].cVar, strValue);
	script->Store(1);

	script->UpdateCompareFlag(result != 0);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_InifileWriteString(CScript* script)
/****************************************************************
Opcode Format
0AF5=4,write_string %1s% to_ini_file %2s% section %3s% key %4s%
****************************************************************/
{
	script->Collect(4);

	char iniPath[MAX_PATH];
	char path[100];
	char sectionName[100];
	char key[100];
	char strValue[100];
	char *strptr;
	BOOL result;

	char* Space = CLEO_GetScriptSpaceAddress();
	switch ((*(tParamType *)(&Space[script->m_dwIp])).type)
	{
	case PARAM_TYPE_STRING:
		strcpy(strValue, Params[0].cVar);
		break;
	default:
		strptr = (char *)Params[0].nVar;
		strcpy(strValue, strptr);
	}
	strcpy(strValue, Params[0].cVar);
	strcpy(path, Params[1].cVar);
	strcpy(sectionName, Params[2].cVar);
	strcpy(key, Params[3].cVar);

	//if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
	MakeFullPath(path, iniPath);

	result = WritePrivateProfileString(sectionName, key, strValue, iniPath);

	script->UpdateCompareFlag(result != 0);

	return OR_CONTINUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		if (CLEO_GetVersion() < CLEO_VERSION)
		{
			MessageBox(HWND_DESKTOP, "An incorrect version of CLEO was loaded.", "IniFiles.cleo", MB_ICONERROR);
			return FALSE;
		}

		Params = CLEO_GetParamsAddress();
		Opcodes::RegisterOpcode(OPCODE_INIFILE_GET_INT, Script_InifileGetInt);
		Opcodes::RegisterOpcode(OPCODE_INIFILE_WRITE_INT, Script_InifileWriteInt);
		Opcodes::RegisterOpcode(OPCODE_INIFILE_GET_FLOAT, Script_InifileGetFloat);
		Opcodes::RegisterOpcode(OPCODE_INIFILE_WRITE_FLOAT, Script_InifileWriteFloat);
		Opcodes::RegisterOpcode(OPCODE_INIFILE_READ_STRING, Script_InifileReadString);
		Opcodes::RegisterOpcode(OPCODE_INIFILE_WRITE_STRING, Script_InifileWriteString);
	}
	return TRUE;
}