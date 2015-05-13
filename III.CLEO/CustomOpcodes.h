#pragma once
#include "CustomScript.h"
#include <Windows.h>

class CustomOpcodes
{
	static eOpcodeResult WINAPI GOTO(CScript *script);
	static eOpcodeResult WINAPI GOTO_IF_TRUE(CScript *script);
	static eOpcodeResult WINAPI GOTO_IF_FALSE(CScript *script);
	static eOpcodeResult WINAPI GOSUB(CScript *script);
	static eOpcodeResult WINAPI TERMINATE_CUSTOM_THREAD(CScript *script);
	static eOpcodeResult WINAPI TERMINATE_NAMED_CUSTOM_THREAD(CScript *script);
	static eOpcodeResult WINAPI START_CUSTOM_THREAD(CScript *script);
	static eOpcodeResult WINAPI MEMORY_WRITE(CScript *script);
	static eOpcodeResult WINAPI MEMORY_READ(CScript *script);
	static eOpcodeResult WINAPI CALL(CScript *script);
	static eOpcodeResult WINAPI CALL_FUNCTION(CScript *script);
	static eOpcodeResult WINAPI CALL_METHOD(CScript *script);
	static eOpcodeResult WINAPI CALL_FUNCTION_METHOD(CScript *script);
	static eOpcodeResult WINAPI GET_GAME_VERSION(CScript *script);
	static eOpcodeResult WINAPI GET_CHAR_STRUCT(CScript *script);
	static eOpcodeResult WINAPI GET_CAR_STRUCT(CScript *script);
	static eOpcodeResult WINAPI GET_OBJECT_STRUCT(CScript *script);
	static eOpcodeResult WINAPI GET_CHAR_HANDLE(CScript *script);
	static eOpcodeResult WINAPI GET_CAR_HANDLE(CScript *script);
	static eOpcodeResult WINAPI GET_OBJECT_HANDLE(CScript *script);
	static eOpcodeResult WINAPI GET_THREAD_POINTER(CScript *script);
	static eOpcodeResult WINAPI GET_NAMED_THREAD_POINTER(CScript *script);
	static eOpcodeResult WINAPI IS_KEY_PRESSED(CScript *script);
	static eOpcodeResult WINAPI FIND_RANDOM_CHAR(CScript *script);
	static eOpcodeResult WINAPI FIND_RANDOM_CAR(CScript *script);
	static eOpcodeResult WINAPI FIND_RANDOM_OBJECT(CScript *script);
	static eOpcodeResult WINAPI CALL_POP_FLOAT(CScript *script);
	static eOpcodeResult WINAPI MATH_EXP(CScript *script);
	static eOpcodeResult WINAPI MATH_LOG(CScript *script);
	static eOpcodeResult WINAPI CALL_SCM_FUNCTION(CScript *script);
	static eOpcodeResult WINAPI SCM_FUNCTION_RET(CScript *script);
	static eOpcodeResult WINAPI GET_LABEL_OFFSET(CScript *script);
	static eOpcodeResult WINAPI GET_VAR_OFFSET(CScript *script);
	static eOpcodeResult WINAPI BIT_AND(CScript *script);
	static eOpcodeResult WINAPI BIT_OR(CScript *script);
	static eOpcodeResult WINAPI BIT_XOR(CScript *script);
	static eOpcodeResult WINAPI BIT_NOT(CScript *script);
	static eOpcodeResult WINAPI BIT_MOD(CScript *script);
	static eOpcodeResult WINAPI BIT_SHR(CScript *script);
	static eOpcodeResult WINAPI BIT_SHL(CScript *script);
	static eOpcodeResult WINAPI START_CUSTOM_THREAD_VSTRING(CScript *script);
public:
	static void Register();
};