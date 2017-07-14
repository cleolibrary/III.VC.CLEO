#include "ScmFunction.h"
#include <Windows.h>

ScmFunction::ScmFunction(CScript *script)
{
	this->prev = script->m_pScmFunction;
	memcpy(this->vars, script->m_aLVars, 64);
}