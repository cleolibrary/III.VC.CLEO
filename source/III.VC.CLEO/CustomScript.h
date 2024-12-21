#pragma once

#ifndef CLEOAPI
#define CLEOAPI __declspec(dllexport)
#endif

#include "ScmFunction.h"
#include <wtypes.h>


enum eScriptType :unsigned short
{
	SCRIPT_TYPE_DEFAULT = 0,
	SCRIPT_TYPE_CUSTOM = 1
};

enum eOpcodeResult :char
{
	OR_CONTINUE = 0,
	OR_TERMINATE = 1,
	OR_UNDEFINED = -1
};

typedef unsigned long DWORD;

union tScriptVar
{
	int nVar;
	float fVar;
	char *cVar;
	void *pVar;
	DWORD dVar;
};

enum eParamType :unsigned char
{
	PARAM_TYPE_END_OF_PARAMS = 0,
	PARAM_TYPE_INT32 = 1,
	PARAM_TYPE_GVAR = 2,
	PARAM_TYPE_LVAR = 3,
	PARAM_TYPE_INT8 = 4,
	PARAM_TYPE_INT16 = 5,
	PARAM_TYPE_FLOAT = 6,
	PARAM_TYPE_STRING = 14
};

struct tParamType
{
	eParamType type : 7; // eParamType
	bool processed : 1; // did we process long string already
};

enum eLogicalOperation : WORD
{
	NONE = 0, // just replace

	AND_2 = 1, // AND operation on results of next two conditional opcodes
	AND_3,
	AND_4,
	AND_5,
	AND_6,
	AND_7,
	AND_END,

	OR_2 = 21, // OR operation on results of next two conditional opcodes
	OR_3,
	OR_4,
	OR_5,
	OR_6,
	OR_7,
	OR_END,
};
static eLogicalOperation& operator--(eLogicalOperation& o)
{
	if (o == eLogicalOperation::NONE) return o; // can not be decremented anymore
	if (o == eLogicalOperation::OR_2) return o = eLogicalOperation::NONE;

	auto val = static_cast<WORD>(o); // to number
	val--;
	return o = static_cast<eLogicalOperation>(val);
}

class CScript
{
public:
	/* 0x00 */ CScript *m_pNext;
	/* 0x04 */ CScript *m_pPrev;
	/* 0x08 */ char m_acName[8];
	/* 0x10 */ unsigned int m_dwIp;
	/* 0x14 */ unsigned int m_aGosubAddr[6];
	/* 0x2C */ unsigned short m_nCurrentGosub;
	/* 0x2E */ unsigned short m_nScriptType; // see eScriptType
	/* 0x30 */ tScriptVar m_aLVars[18];
#if CLEO_VC
	/* 0x78 */ bool m_bIsActive; 
	/* 0x79 */ bool m_bCondResult; 
	/* 0x7A */ bool m_bIsMission;
#else
	/* 0x78 */ bool m_bCondResult;
	/* 0x79 */ bool m_bIsMission;
	/* 0x7A */ bool m_bIsActive;
#endif
	/* 0x7B */ bool m_bAwake;
	/* 0x7C */ unsigned int m_dwWakeTime;
	/* 0x80 */ eLogicalOperation m_wIfOp;
	/* 0x82 */ bool m_bNotFlag;
	/* 0x83 */ bool m_bDeathArrestCheckEnabled;
	/* 0x84 */ bool m_bWastedOrBusted;
	/* 0x85 */ bool m_bMissionFlag;
	/* 0x86 */ short _pad;

	/* Custom data */
	/* 0x88 */ unsigned int m_nLastPedSearchIndex;
	/* 0x8C */ unsigned int m_nLastVehicleSearchIndex;
	/* 0x90 */ unsigned int m_nLastObjectSearchIndex;
	/* 0x94 */ struct _LoadingErrors{
				   unsigned int m_bFileNotFound : 1;
				   unsigned int m_bFileSeekError : 1;
				   unsigned int m_bInternalError : 1;
				   unsigned int m_bEmptyFile : 1;
				   unsigned int m_bAllocationFailed : 1;
				   unsigned int m_bEofReached : 1;
			   } m_Errors;
	/* 0x98 */ char *m_pCodeData;
	/* 0x9C */ unsigned int m_dwBaseIp;
	/* 0xA0 */ class ScmFunction *m_pScmFunction;
	/* 0xA4 */ CScript *m_pNextCustom;
	/* 0xA8 */ CScript *m_pPrevCustom;
	/* 0xAC */ tScriptVar *m_pLocalArray;

	void Init();

	bool Loaded();

	void AddToCustomList(CScript **list);

	void RemoveFromCustomList(CScript **list);

	CLEOAPI void Collect(unsigned int numParams);

	CLEOAPI void Collect(unsigned int *pIp, unsigned int numParams);

	CLEOAPI int CollectNextWithoutIncreasingPC(unsigned int ip);

	CLEOAPI eParamType GetNextParamType();

	CLEOAPI void Store(unsigned int numParams);

	CLEOAPI void ReadShortString(char *out);

	CLEOAPI void UpdateCompareFlag(bool result);

	CLEOAPI void *GetPointerToScriptVariable();

	CScript() = default;

	CScript(char *filepath);

	~CScript();

	CLEOAPI void JumpTo(int address);

	eOpcodeResult ProcessOneCommand();
};

static_assert(sizeof(CScript) == 0xB0, "Error with CScript");