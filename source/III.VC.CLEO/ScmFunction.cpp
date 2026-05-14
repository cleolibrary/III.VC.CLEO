#include "ScmFunction.h"
#include "CustomScript.h"
#include <Windows.h>

ScmFunction::ScmFunction(CScript *script)
{
	prev = script->m_pScmFunction;
	script->m_pScmFunction = this;

	// create snapshot of current scope
	retAddr = script->m_dwIp;

	savedCondResult = script->m_bCondResult;
	savedLogicalOp = script->m_wIfOp;
	savedNotFlag = script->m_bNotFlag;

	// store local variables
	memcpy(vars, script->m_aLVars, sizeof(vars));

	// store GOSUB stack
	memcpy(savedStack, script->m_aGosubAddr, sizeof(savedStack));
	savedSP = script->m_nCurrentGosub;

	// init new scope
	script->m_bCondResult = false;
	script->m_wIfOp = eLogicalOperation::NONE;
	script->m_bNotFlag = false;

	ZeroMemory(script->m_aLVars, 16 * sizeof(tScriptVar)); // keep timers

	ZeroMemory(script->m_aGosubAddr, sizeof(script->m_aGosubAddr));
	script->m_nCurrentGosub = 0;
}

void ScmFunction::Return(CScript* script)
{
	ScmFunction& func = *script->m_pScmFunction;

	// process conditional result of just ended function in parent scope
	bool condResult = script->m_bCondResult;
	if (func.savedNotFlag) condResult = !condResult;

	if (func.savedLogicalOp >= eLogicalOperation::AND_2 && func.savedLogicalOp < eLogicalOperation::AND_END)
	{
		script->m_bCondResult = func.savedCondResult && condResult;
		script->m_wIfOp = --func.savedLogicalOp;
	}
	else if (func.savedLogicalOp >= eLogicalOperation::OR_2 && func.savedLogicalOp < eLogicalOperation::OR_END)
	{
		script->m_bCondResult = func.savedCondResult || condResult;
		script->m_wIfOp = --func.savedLogicalOp;
	}
	else // eLogicalOperation::NONE
	{
		script->m_bCondResult = condResult;
		script->m_wIfOp = func.savedLogicalOp;
	}

	// restore parent scope's local variables
	memcpy(script->m_aLVars, func.vars, sizeof(vars));

	// restore parent scope's gosub call stack
	memcpy(script->m_aGosubAddr, func.savedStack, sizeof(savedStack));
	script->m_nCurrentGosub = func.savedSP;

	script->m_dwIp = func.retAddr;
	script->m_pScmFunction = func.prev;

	delete &func;
}
