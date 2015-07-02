#include "stdafx.h"
#include <stdio.h>
#if _VC
#pragma comment (lib, "..\\..\\..\\..\\Release\\VC.CLEO.lib")
#include "..\..\..\VC.CLEO.h"
#else if _III
#pragma comment (lib, "..\\..\\..\\..\\Release\\III.CLEO.lib")
#include "..\..\..\III.CLEO.h"
#endif

#define OPCODE_FS_DELETEFILE		0x0B00
#define OPCODE_FS_DELETEDIR			0x0B01
#define OPCODE_FS_MOVEFILE			0x0B02
#define OPCODE_FS_MOVEDIR			0x0B03
#define OPCODE_FS_COPYFILE			0x0B04
#define OPCODE_FS_COPYDIR			0x0B05

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

eOpcodeResult WINAPI Script_FS_DeleteFile(CScript* script)
/****************************************************************
Opcode Format
0B00=1,delete_file %1d% //IF and SET
****************************************************************/
{
	script->Collect(1);
	script->UpdateCompareFlag(DeleteFile(Params[0].cVar));
	return OR_CONTINUE;
}

BOOL DeleteDir(const char *path)
{
	char mask[MAX_PATH];
	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	char subPath[MAX_PATH];

	memset(&wfd, 0, sizeof(wfd));
	//search mask
	sprintf(mask, "%s\\*", path);

	//try to delete all inside first
	if ((hSearch = FindFirstFile(mask, &wfd)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			//recursively delete subdirectories
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((strcmp(wfd.cFileName, "..") != 0) && (strcmp(wfd.cFileName, ".") != 0))
				{
					sprintf(subPath, "%s\\%s", path, wfd.cFileName);
					if (!DeleteDir(subPath))
						return FALSE;
				}
			}
			else
			{
				//remove read-only, system, hidden flags
				sprintf(subPath, "%s\\%s", path, wfd.cFileName);
				SetFileAttributes(subPath, FILE_ATTRIBUTE_NORMAL);
				//delete file
				if (!DeleteFile(subPath))
					return FALSE;
			}


		} while (FindNextFile(hSearch, &wfd));
		FindClose(hSearch);
	}

	//delete empty directory
	return RemoveDirectory(path);
}

eOpcodeResult WINAPI Script_FS_DeleteDirectory(CScript* script)
/****************************************************************
Opcode Format
0B01=2,delete_directory %1d% with_all_files_and_subdirectories %2d% //IF and SET
****************************************************************/
{
	script->Collect(2);
	int DeleteAllInsideFlag;
	BOOL result;

	DeleteAllInsideFlag = Params[1].nVar;

	if (DeleteAllInsideFlag)
	{
		//remove directory with all files and subdirectories
		result = DeleteDir(Params[0].cVar);
	}
	else
	{
		//try to remove as empty directory
		result = RemoveDirectory(Params[0].cVar);
	}

	script->UpdateCompareFlag(result);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_FS_MoveFile(CScript* script)
/****************************************************************
Opcode Format
0B02=2,move_file %1d% to %2d% //IF and SET
****************************************************************/
{
	script->Collect(2);
	BOOL result;

	result = GetFileAttributes(Params[0].cVar) & FILE_ATTRIBUTE_DIRECTORY;
	if (!result)
		result = MoveFile(Params[0].cVar, Params[1].cVar);

	script->UpdateCompareFlag(result);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_FS_MoveDir(CScript* script)
/****************************************************************
Opcode Format
0B03=2,move_directory %1d% to %2d% //IF and SET
****************************************************************/
{
	script->Collect(2);
	BOOL result;

	result = GetFileAttributes(Params[0].cVar) & FILE_ATTRIBUTE_DIRECTORY;
	if (result)
		result = MoveFile(Params[0].cVar, Params[1].cVar);

	script->UpdateCompareFlag(result);

	return OR_CONTINUE;
}

eOpcodeResult WINAPI Script_FS_CopyFile(CScript* script)
/****************************************************************
Opcode Format
0B04=2,copy_file %1d% to %2d% //IF and SET
****************************************************************/
{
	script->Collect(2);
	BOOL result;
	DWORD fattr;

	if (result = CopyFile(Params[0].cVar, Params[1].cVar, FALSE))
	{
		//copy file attributes
		fattr = GetFileAttributes(Params[0].cVar);
		SetFileAttributes(Params[1].cVar, fattr);
	}
	script->UpdateCompareFlag(result);

	return OR_CONTINUE;
}

BOOL CopyDir(const char *path, const char *newPath)
{
	char mask[MAX_PATH];
	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	char subPath[MAX_PATH], newSubPath[MAX_PATH];
	DWORD fattr;

	//create parent directory
	if (!CreateDirectory(newPath, NULL))
		return FALSE;

	memset(&wfd, 0, sizeof(wfd));
	//search mask
	sprintf(mask, "%s\\*", path);

	//copy all files and folders into new directory
	if ((hSearch = FindFirstFile(mask, &wfd)) != INVALID_HANDLE_VALUE)
	{
		do
		{
			sprintf(subPath, "%s\\%s", path, wfd.cFileName);
			sprintf(newSubPath, "%s\\%s", newPath, wfd.cFileName);
			//copy subdirectories
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((strcmp(wfd.cFileName, "..") != 0) && (strcmp(wfd.cFileName, ".") != 0))
				{
					if (!CopyDir(subPath, newSubPath))
						return FALSE;
				}
			}
			else
			{
				//copy file into new directory
				if (CopyFile(subPath, newSubPath, FALSE))
				{
					fattr = GetFileAttributes(subPath);
					SetFileAttributes(newSubPath, fattr);
				}
				else	return FALSE;
			}


		} while (FindNextFile(hSearch, &wfd));
		FindClose(hSearch);
	}

	//return success
	return TRUE;
}

eOpcodeResult WINAPI Script_FS_CopyDir(CScript* script)
/****************************************************************
Opcode Format
0B05=2,copy_directory %1d% to %2d% //IF and SET
****************************************************************/
{
	script->Collect(2);
	script->UpdateCompareFlag(CopyDir(Params[0].cVar, Params[1].cVar));

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
		Opcodes::RegisterOpcode(OPCODE_FS_DELETEFILE, Script_FS_DeleteFile);
		Opcodes::RegisterOpcode(OPCODE_FS_DELETEDIR, Script_FS_DeleteDirectory);
		Opcodes::RegisterOpcode(OPCODE_FS_MOVEFILE, Script_FS_MoveFile);
		Opcodes::RegisterOpcode(OPCODE_FS_MOVEDIR, Script_FS_MoveDir);
		Opcodes::RegisterOpcode(OPCODE_FS_COPYFILE, Script_FS_CopyFile);
		Opcodes::RegisterOpcode(OPCODE_FS_COPYDIR, Script_FS_CopyDir);
	}
	return TRUE;
}