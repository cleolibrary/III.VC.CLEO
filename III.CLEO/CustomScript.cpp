#include "CustomScript.h"
#include "Game.h"
#include "OpcodesSystem.h"
#include <Windows.h>
#include <stdio.h>
#include "ScriptManager.h"
#include "Log.h"

void CScript::Init()
{
	memset(this, 0, sizeof(CScript));
	strcpy(this->m_acName, "noname");
	this->m_bDeathArrestCheckEnabled = true;
}

CScript::CScript()
{

}

bool CScript::Loaded()
{
	if(this->m_Errors.m_bAllocationFailed || this->m_Errors.m_bEmptyFile || this->m_Errors.m_bEofReached
		|| this->m_Errors.m_bFileNotFound || this->m_Errors.m_bFileSeekError || this->m_Errors.m_bInternalError)
		return false;
	return true;
}

CScript::~CScript()
{
	ScmFunction *scmf = this->m_pScmFunction;
	while(scmf)
	{
		ScmFunction *prev = scmf->prev;
		delete scmf;
		scmf = prev;
	}
}

CScript::CScript(char *filepath)
{
	this->Init();
	this->m_nScriptType = SCRIPT_TYPE_CUSTOM;
	FILE *file = fopen(filepath, "rb");
	if(!file)
	{
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"File Not Found\"", filepath);
		this->m_Errors.m_bFileNotFound = true;
		return;
	}
	if(fseek(file, 0, SEEK_END))
	{
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"File Seek Error\"", filepath);
		fclose(file);
		this->m_Errors.m_bFileSeekError = true;
		return;
	}
	unsigned int filesize = ftell(file);
	if(!filesize)
	{
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Empty File\"", filepath);
		fclose(file);
		this->m_Errors.m_bEmptyFile = true;
		return;
	}
	if(fseek(file, 0, SEEK_SET))
	{
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"File Seek Error\"", filepath);
		fclose(file);
		this->m_Errors.m_bFileSeekError = true;
		return;
	}
	this->m_pCodeData = scriptMgr.AllocateMemoryForScript(filepath, filesize);
	if(!this->m_pCodeData)
	{
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Allocation Failed\"", filepath);
		fclose(file);
		this->m_Errors.m_bAllocationFailed = true;
		return;
	}
	this->m_dwBaseIp = (unsigned int)this->m_pCodeData - (unsigned int)game.Scripts.Space;
	this->m_bMissionFlag = 0;
	this->m_dwIp = this->m_dwBaseIp;
	if(!fread(this->m_pCodeData, 1, filesize, file))
	{
		if(ferror(file))
		{
			LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Internal Error\"", filepath);
			scriptMgr.DeleteScriptMemory(filepath, this->m_pCodeData);
			this->m_pCodeData = 0;
			this->m_dwBaseIp = this->m_dwIp = 0;
			this->m_Errors.m_bInternalError = true;
			fclose(file);
			return;
		}
		else if(feof(file))
		{
			LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"EOF Reached\"", filepath);
			scriptMgr.DeleteScriptMemory(filepath, this->m_pCodeData);
			this->m_pCodeData = 0;
			this->m_dwBaseIp = this->m_dwIp = 0;
			this->m_Errors.m_bEofReached = true;
			fclose(file);
			return;
		}
	}
	if(fclose(file) == -1)
	{
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Error(script loading): %s, \"Internal Error\"", filepath);
		scriptMgr.DeleteScriptMemory(filepath, this->m_pCodeData);
		this->m_pCodeData = 0;
		this->m_dwBaseIp = this->m_dwIp = 0;
		this->m_Errors.m_bInternalError = true;
		fclose(file);
		return;
	}
	strncpy(this->m_acName, &strrchr(filepath, '\\')[1], 8);
	this->m_pPrevCustom = NULL;
	this->m_pNextCustom = NULL;
	memset(&this->m_Errors, 0, 4);
	this->m_acName[7] = 0;
	LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Loaded script \"%s\" from \"%s\"", this->m_acName, filepath);
}

void CScript::ReadShortString(char *out)
{
	strncpy(out, &game.Scripts.Space[this->m_dwIp], 7);
	out[7] = '\0';
	this->m_dwIp += 8;
}

void CScript::AddToCustomList(CScript **list)
{
	this->m_pNextCustom = *list;
	this->m_pPrevCustom = 0;
	if(*list)
		(*list)->m_pPrevCustom = this;
	*list = this;
}

void CScript::RemoveFromCustomList(CScript **list)
{
	if(this->m_pPrevCustom)
		this->m_pPrevCustom->m_pNextCustom = this->m_pNextCustom;
	else
		*list = this->m_pNextCustom;
	if(this->m_pNextCustom)
		this->m_pNextCustom->m_pPrevCustom = this->m_pPrevCustom;
}

void CScript::JumpTo(int address)
{
	if(address >= 0)
		this->m_dwIp = address;
	else
	{
		if(this->m_nScriptType == SCRIPT_TYPE_CUSTOM)
			this->m_dwIp = this->m_dwBaseIp - address;
		else
			this->m_dwIp = 0x20000 - address;
	}
}

eParamType CScript::GetNextParamType()
{
	tParamType paramType = *(tParamType *)&game.Scripts.Space[this->m_dwIp];
	if(paramType.isString)
		return PARAM_TYPE_STRING;
	return (eParamType)paramType.type;
}

void CScript::Collect(unsigned int numParams)
{
	this->Collect(&this->m_dwIp, numParams);
}

void CScript::Collect(unsigned int *pIp, unsigned int numParams)
{
	for(int i = 0; i < numParams; i++)
	{
		unsigned char paramType = *(unsigned char *)&game.Scripts.Space[*pIp];
		*pIp += 1;
		// if string parameter
		if((*(tParamType *)&paramType).isString)
		{
			game.Scripts.Params[i].cVar = &game.Scripts.Space[*pIp];
			*pIp += (*(tParamTypeString *)&paramType).length + 1;
		}
		else
		{
			unsigned int j;
			unsigned char *pLength;
			unsigned char length;
			switch((*(tParamType *)&paramType).type)
			{
			case PARAM_TYPE_INT32:
				game.Scripts.Params[i].nVar = *(int *)&game.Scripts.Space[*pIp];
				*pIp += 4;
				break;
			case PARAM_TYPE_GVAR:
				game.Scripts.Params[i].nVar = *(int *)&game.Scripts.Space[*(unsigned short *)&game.Scripts.Space[*pIp]];
				*pIp += 2;
				break;
			case PARAM_TYPE_LVAR:
				game.Scripts.Params[i].nVar = this->m_aLVars[*(unsigned short *)&game.Scripts.Space[*pIp]].nVar;
				*pIp += 2;
				break;
			case PARAM_TYPE_INT8:
				game.Scripts.Params[i].nVar = *(char *)&game.Scripts.Space[*pIp];
				*pIp += 1;
				break;
			case PARAM_TYPE_INT16:
				game.Scripts.Params[i].nVar = *(short *)&game.Scripts.Space[*pIp];
				*pIp += 2;
				break;
			case PARAM_TYPE_FLOAT:
				game.Scripts.Params[i].fVar = (float)(*(short *)&game.Scripts.Space[*pIp]) / 16.0f;
				*pIp += 2;
				break;
			case PARAM_TYPE_STRING:
				pLength = (unsigned char *)&game.Scripts.Space[*pIp];
				length = *pLength;
				if((*(tParamType *)&paramType).isLongString)
				{
					game.Scripts.Params[i].cVar = &game.Scripts.Space[*pIp + 1];
					*pIp += length + 2;
				}
				else
				{
					if(length > 127)
					{
						(*(tParamType *)&game.Scripts.Space[*pIp - 1]).isString = false;
						(*(tParamType *)&game.Scripts.Space[*pIp - 1]).isLongString = true;
						game.Scripts.Space[*pIp + length] = '\0';
						game.Scripts.Params[i].cVar = &game.Scripts.Space[*pIp + 1];
						*pIp += length + 1;
						*pLength = length - 1;
					}
					else
					{
						(*(tParamTypeString *)&game.Scripts.Space[*pIp - 1]).isString = true;
						(*(tParamTypeString *)&game.Scripts.Space[*pIp - 1]).length = length;
						for(j = 0; j < length; j++)
							game.Scripts.Space[*pIp + j] = game.Scripts.Space[*pIp + 1 + j];
						game.Scripts.Space[*pIp + length] = '\0';
						game.Scripts.Params[i].cVar = &game.Scripts.Space[*pIp];
						*pIp += length + 1;
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

int CScript::CollectNextWithoutIncreasingPC(unsigned int ip)
{
	unsigned char paramType = *(unsigned char *)&game.Scripts.Space[ip];
	ip += 1;
	// if string parameter
	if((*(tParamType *)&paramType).isString)
		return (int)&game.Scripts.Space[ip];
	else
	{
		unsigned int j;
		unsigned char *pLength;
		unsigned char length;
		float fParam;
		switch((*(tParamType *)&paramType).type)
		{
		case PARAM_TYPE_INT32:
			return *(int *)&game.Scripts.Space[ip];
		case PARAM_TYPE_GVAR:
			return *(int *)&game.Scripts.Space[*(unsigned short *)&game.Scripts.Space[ip]];
		case PARAM_TYPE_LVAR:
			return this->m_aLVars[*(unsigned short *)&game.Scripts.Space[ip]].nVar;
		case PARAM_TYPE_INT8:
			return *(char *)&game.Scripts.Space[ip];
		case PARAM_TYPE_INT16:
			return *(short *)&game.Scripts.Space[ip];
		case PARAM_TYPE_FLOAT:
			fParam = (float)(*(short *)&game.Scripts.Space[ip]) / 16.0f;
			return *(int *)&fParam;
		case PARAM_TYPE_STRING:
			pLength = (unsigned char *)&game.Scripts.Space[ip];
			length = *pLength;
			if((*(tParamType *)&paramType).isLongString)
				return (int)&game.Scripts.Space[ip + 1];
			else
			{
				if(length > 127)
				{
					(*(tParamType *)&game.Scripts.Space[ip - 1]).isString = false;
					(*(tParamType *)&game.Scripts.Space[ip - 1]).isLongString = true;
					game.Scripts.Space[ip + length] = '\0';
					*pLength = length - 1;
					return (int)&game.Scripts.Space[ip + 1];
				}
				else
				{
					(*(tParamTypeString *)&game.Scripts.Space[ip - 1]).isString = true;
					(*(tParamTypeString *)&game.Scripts.Space[ip - 1]).length = length;
					for(j = 0; j < length; j++)
						game.Scripts.Space[ip + j] = game.Scripts.Space[ip + 1 + j];
					game.Scripts.Space[ip + length] = '\0';
					return (int)&game.Scripts.Space[ip];
				}
			}
			break;
		default:
			return -1;
		}
	}
}

void CScript::Store(unsigned int numParams)
{
	game.Scripts.StoreParameters(this, &this->m_dwIp, numParams);
}

void CScript::UpdateCompareFlag(bool result)
{
	game.Scripts.UpdateCompareFlag(this, result);
}

void *CScript::GetPointerToScriptVariable()
{
	return game.Scripts.GetPointerToScriptVariable(this, &this->m_dwIp, 1);
}

eOpcodeResult CScript::ProcessOneCommand()
{
	*game.Scripts.pNumOpcodesExecuted += 1;
	unsigned short id = *(unsigned short *)&game.Scripts.Space[this->m_dwIp] & 0x7FFF;
	if(*(unsigned short *)&game.Scripts.Space[this->m_dwIp] & 0x8000)
		this->m_bNotFlag = true;
	else
		this->m_bNotFlag = false;
	this->m_dwIp += 2;
	// check for custom opcodes here
	if(Opcodes::functions[id])
	{
		// call custom opcode
		LOGL(LOG_PRIORITY_OPCODE_ID, "%s custom opcode %04X", this->m_acName, id);
		return Opcodes::functions[id](this);
	}
	else if(id >= CUSTOM_OPCODE_START_ID)
	{
		LOGL(LOG_PRIORITY_ALWAYS, "Error (incorrect opcode): %s, %04X", this->m_acName, id);
		Error("Incorrect opcode ID: %04X", id);
		return OR_UNDEFINED;
	}
	// call default opcode
	LOGL(LOG_PRIORITY_OPCODE_ID, "%s opcode %04X", this->m_acName, id);
	eOpcodeResult result = game.Scripts.OpcodeHandlers[id / 100](this, id);
	return result;
}