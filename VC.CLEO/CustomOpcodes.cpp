#include "CustomOpcodes.h"
#include "Game.h"
#include "OpcodesSystem.h"
#include "ScriptManager.h"
#include "Log.h"
#include "Fxt.h"

void CustomOpcodes::Register()
{
    Opcodes::RegisterOpcode(0x0002, GOTO);
    Opcodes::RegisterOpcode(0x004C, GOTO_IF_TRUE);
    Opcodes::RegisterOpcode(0x004D, GOTO_IF_FALSE);
    Opcodes::RegisterOpcode(0x0050, GOSUB);
    Opcodes::RegisterOpcode(0x05DC, TERMINATE_CUSTOM_THREAD);
    Opcodes::RegisterOpcode(0x05DD, TERMINATE_NAMED_CUSTOM_THREAD);
    Opcodes::RegisterOpcode(0x05DE, START_CUSTOM_THREAD);
    Opcodes::RegisterOpcode(0x05DF, MEMORY_WRITE);
    Opcodes::RegisterOpcode(0x05E0, MEMORY_READ);
    Opcodes::RegisterOpcode(0x05E1, CALL);
    Opcodes::RegisterOpcode(0x05E2, CALL_FUNCTION);
    Opcodes::RegisterOpcode(0x05E3, CALL_METHOD);
    Opcodes::RegisterOpcode(0x05E4, CALL_FUNCTION_METHOD);
    Opcodes::RegisterOpcode(0x05E5, GET_GAME_VERSION);
    Opcodes::RegisterOpcode(0x05E6, GET_CHAR_STRUCT);
    Opcodes::RegisterOpcode(0x05E7, GET_CAR_STRUCT);
    Opcodes::RegisterOpcode(0x05E8, GET_OBJECT_STRUCT);
    Opcodes::RegisterOpcode(0x05E9, GET_CHAR_HANDLE);
    Opcodes::RegisterOpcode(0x05EA, GET_CAR_HANDLE);
    Opcodes::RegisterOpcode(0x05EB, GET_OBJECT_HANDLE);
    Opcodes::RegisterOpcode(0x05EC, GET_THREAD_POINTER);
    Opcodes::RegisterOpcode(0x05ED, GET_NAMED_THREAD_POINTER);
    Opcodes::RegisterOpcode(0x05EE, IS_KEY_PRESSED);
    Opcodes::RegisterOpcode(0x05EF, FIND_RANDOM_CHAR);
    Opcodes::RegisterOpcode(0x05F0, FIND_RANDOM_CAR);
    Opcodes::RegisterOpcode(0x05F1, FIND_RANDOM_OBJECT);
    Opcodes::RegisterOpcode(0x05F2, CALL_POP_FLOAT);
    Opcodes::RegisterOpcode(0x05F3, MATH_EXP);
    Opcodes::RegisterOpcode(0x05F4, MATH_LOG);
    Opcodes::RegisterOpcode(0x05F5, CALL_SCM_FUNCTION);
    Opcodes::RegisterOpcode(0x05F6, SCM_FUNCTION_RET);
    Opcodes::RegisterOpcode(0x05F7, GET_LABEL_OFFSET);
    Opcodes::RegisterOpcode(0x05F8, GET_VAR_OFFSET);
    Opcodes::RegisterOpcode(0x05F9, BIT_AND);
    Opcodes::RegisterOpcode(0x05FA, BIT_OR);
    Opcodes::RegisterOpcode(0x05FB, BIT_XOR);
    Opcodes::RegisterOpcode(0x05FC, BIT_NOT);
    Opcodes::RegisterOpcode(0x05FD, BIT_MOD);
    Opcodes::RegisterOpcode(0x05FE, BIT_SHR);
    Opcodes::RegisterOpcode(0x05FF, BIT_SHL);

	//Duplicate CLEO3/VC opcodes to match CLEO4 SA ids
	Opcodes::RegisterOpcode(0x0A93, TERMINATE_CUSTOM_THREAD);
	Opcodes::RegisterOpcode(0x0ABA, TERMINATE_NAMED_CUSTOM_THREAD);
	Opcodes::RegisterOpcode(0x0A92, START_CUSTOM_THREAD);
	Opcodes::RegisterOpcode(0x0A8C, MEMORY_WRITE);
	Opcodes::RegisterOpcode(0x0A8D, MEMORY_READ);
	Opcodes::RegisterOpcode(0x0AA5, CALL);
	Opcodes::RegisterOpcode(0x0AA7, CALL_FUNCTION);
	Opcodes::RegisterOpcode(0x0AA6, CALL_METHOD);
	Opcodes::RegisterOpcode(0x0AA8, CALL_FUNCTION_METHOD);
	Opcodes::RegisterOpcode(0x0A96, GET_CHAR_STRUCT);
	Opcodes::RegisterOpcode(0x0A97, GET_CAR_STRUCT);
	Opcodes::RegisterOpcode(0x0A98, GET_OBJECT_STRUCT);
	Opcodes::RegisterOpcode(0x0AEA, GET_CHAR_HANDLE);
	Opcodes::RegisterOpcode(0x0AEB, GET_CAR_HANDLE);
	Opcodes::RegisterOpcode(0x0AEC, GET_OBJECT_HANDLE);
	Opcodes::RegisterOpcode(0x0A9F, GET_THREAD_POINTER);
	Opcodes::RegisterOpcode(0x0AAA, GET_NAMED_THREAD_POINTER);
	Opcodes::RegisterOpcode(0x0AB0, IS_KEY_PRESSED);
	Opcodes::RegisterOpcode(0x0AE1, FIND_RANDOM_CHAR);
	Opcodes::RegisterOpcode(0x0AE2, FIND_RANDOM_CAR);
	Opcodes::RegisterOpcode(0x0AE3, FIND_RANDOM_OBJECT);
	Opcodes::RegisterOpcode(0x0AE9, CALL_POP_FLOAT);
	Opcodes::RegisterOpcode(0x0AEE, MATH_EXP);
	Opcodes::RegisterOpcode(0x0AEF, MATH_LOG);
	Opcodes::RegisterOpcode(0x0AB1, CALL_SCM_FUNCTION);
	Opcodes::RegisterOpcode(0x0AB2, SCM_FUNCTION_RET);
	Opcodes::RegisterOpcode(0x0AC6, GET_LABEL_OFFSET);
	Opcodes::RegisterOpcode(0x0AC7, GET_VAR_OFFSET);
	Opcodes::RegisterOpcode(0x0B10, BIT_AND);
	Opcodes::RegisterOpcode(0x0B11, BIT_OR);
	Opcodes::RegisterOpcode(0x0B12, BIT_XOR);
	Opcodes::RegisterOpcode(0x0B13, BIT_NOT);
	Opcodes::RegisterOpcode(0x0B14, BIT_MOD);
	Opcodes::RegisterOpcode(0x0B15, BIT_SHR);
	Opcodes::RegisterOpcode(0x0B16, BIT_SHL);

    //CLEO 2 opcodes
    Opcodes::RegisterOpcode(0x0600, START_CUSTOM_THREAD_VSTRING);

    // CLEO 3-4 SA opcodes
    Opcodes::RegisterOpcode(0x0A8E, OPCODE_0A8E);
    Opcodes::RegisterOpcode(0x0A8F, OPCODE_0A8F);
    Opcodes::RegisterOpcode(0x0A90, OPCODE_0A90);
    Opcodes::RegisterOpcode(0x0A91, OPCODE_0A91);
    Opcodes::RegisterOpcode(0x0ADC, OPCODE_0ADC);
    Opcodes::RegisterOpcode(0x0ADF, OPCODE_0ADF);
    Opcodes::RegisterOpcode(0x0AE0, OPCODE_0AE0);
	Opcodes::RegisterOpcode(0x0AA2, OPCODE_0AA2);
	Opcodes::RegisterOpcode(0x0AA3, OPCODE_0AA3);
	Opcodes::RegisterOpcode(0x0AA4, OPCODE_0AA4);
	Opcodes::RegisterOpcode(0x0AAB, OPCODE_0AAB);

	Opcodes::RegisterOpcode(0x0ACA, OPCODE_0ACA);
	Opcodes::RegisterOpcode(0x0ACB, OPCODE_0ACB);
	Opcodes::RegisterOpcode(0x0ACC, OPCODE_0ACC);
	Opcodes::RegisterOpcode(0x0ACD, OPCODE_0ACD);
	/*Opcodes::RegisterOpcode(0x0ACE, OPCODE_0ACE);
	Opcodes::RegisterOpcode(0x0ACF, OPCODE_0ACF);
	Opcodes::RegisterOpcode(0x0AD0, OPCODE_0AD0);
	Opcodes::RegisterOpcode(0x0AD1, OPCODE_0AD1);*/


    //CLEO 4 Ini Files
    Opcodes::RegisterOpcode(0x0AF0, OPCODE_0AF0);
    Opcodes::RegisterOpcode(0x0AF1, OPCODE_0AF1);
    Opcodes::RegisterOpcode(0x0AF2, OPCODE_0AF2);
    Opcodes::RegisterOpcode(0x0AF3, OPCODE_0AF3);
    Opcodes::RegisterOpcode(0x0AF4, OPCODE_0AF4);
    Opcodes::RegisterOpcode(0x0AF5, OPCODE_0AF5);

    //Scrapped opcodes (VC)
    Opcodes::RegisterOpcode(0x016F, DRAW_SHADOW);
    Opcodes::RegisterOpcode(0x0349, SET_TEXT_DRAW_FONT);
}

eOpcodeResult CustomOpcodes::GOTO(CScript *script)
{
    script->Collect(1);
    script->JumpTo(game.Scripts.Params[0].nVar);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GOTO_IF_TRUE(CScript *script)
{
    script->Collect(1);
    if(script->m_bCondResult)
        script->JumpTo(game.Scripts.Params[0].nVar);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GOTO_IF_FALSE(CScript *script)
{
    script->Collect(1);
    if(!script->m_bCondResult)
        script->JumpTo(game.Scripts.Params[0].nVar);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GOSUB(CScript *script)
{
    script->Collect(1);
    script->m_aGosubAddr[script->m_nCurrentGosub++] = script->m_dwIp;
    script->JumpTo(game.Scripts.Params[0].nVar);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::TERMINATE_CUSTOM_THREAD(CScript *script)
{
    game.Scripts.RemoveScriptFromList(script, game.Scripts.pActiveScriptsList);
    script->RemoveFromCustomList(&scriptMgr.pCusomScripts);
    scriptMgr.numLoadedCustomScripts--;
    LOGL(LOG_PRIORITY_OPCODE, "TERMINATE_CUSTOM_THREAD: Terminating custom script \"%s\"", script->m_acName);
    delete script;
    return OR_TERMINATE;
}

eOpcodeResult CustomOpcodes::TERMINATE_NAMED_CUSTOM_THREAD(CScript *script)
{
    char name[8];
    script->ReadShortString(name);
    CScript *search = scriptMgr.pCusomScripts;
    eOpcodeResult result = OR_CONTINUE;
    bool found = false;
    while(search)
    {
        CScript *next = search->m_pNextCustom;
        if(!stricmp(search->m_acName, name))
        {
            if(search == script)
                result = OR_TERMINATE;
            game.Scripts.RemoveScriptFromList(search, game.Scripts.pActiveScriptsList);
            search->RemoveFromCustomList(&scriptMgr.pCusomScripts);
            scriptMgr.numLoadedCustomScripts--;
            LOGL(LOG_PRIORITY_OPCODE, "TERMINATE_NAMED_CUSTOM_THREAD: Terminating custom script with name \"%s\"", search->m_acName);
            delete search;
            found = true;
        }
        search = next;
    }
    if(found)
        script->UpdateCompareFlag(true);
    else
        script->UpdateCompareFlag(false);
    return result;
}

eOpcodeResult CustomOpcodes::START_CUSTOM_THREAD(CScript *script)
{
    char name[8];
    script->ReadShortString(name);
    char filepath[MAX_PATH];
    sprintf(filepath, "%s%.8s", "cleo\\", name);
    CScript *newScript = new CScript(filepath);
    if(newScript && newScript->Loaded())
    {
        newScript->AddToCustomList(&scriptMgr.pCusomScripts);
        scriptMgr.numLoadedCustomScripts++;
        game.Scripts.AddScriptToList(newScript, game.Scripts.pActiveScriptsList);
        newScript->m_bIsActive = true;
        LOGL(LOG_PRIORITY_OPCODE, "START_CUSTOM_THREAD: Started new script \"%s\"", name);
        for(int i = 0; (*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type; i++)
        {
            script->Collect(1);
            newScript->m_aLVars[i].nVar = game.Scripts.Params[0].nVar;
        }
        script->UpdateCompareFlag(true);
    }
    else
    {
        while((*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type)
            script->Collect(1);
        LOGL(LOG_PRIORITY_OPCODE, "START_CUSTOM_THREAD: Script loading failed, \"%s\"", name);
        if(newScript)
            delete newScript;
        script->UpdateCompareFlag(false);
    }
    script->m_dwIp++;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::MEMORY_WRITE(CScript *script)
{
    script->Collect(4);
    DWORD flOldProtect;
    if(game.Scripts.Params[3].nVar)
        VirtualProtect(game.Scripts.Params[0].pVar, game.Scripts.Params[1].nVar, PAGE_EXECUTE_READWRITE, &flOldProtect);
    switch(game.Scripts.Params[1].nVar)
    {
    case 1:
        *(char *)game.Scripts.Params[0].pVar = game.Scripts.Params[2].nVar;
        break;
    case 2:
        *(short *)game.Scripts.Params[0].pVar = game.Scripts.Params[2].nVar;
        break;
    case 4:
        *(int *)game.Scripts.Params[0].pVar = game.Scripts.Params[2].nVar;
        break;
    default:
        memset(game.Scripts.Params[0].pVar, game.Scripts.Params[2].nVar, game.Scripts.Params[1].nVar);
        break;
    }
    if(game.Scripts.Params[3].nVar)
        VirtualProtect(game.Scripts.Params[0].pVar, game.Scripts.Params[1].nVar, flOldProtect, &flOldProtect);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::MEMORY_READ(CScript *script)
{
    script->Collect(3);
    DWORD flOldProtect;
    void *pMemory = game.Scripts.Params[0].pVar;
    if(game.Scripts.Params[2].nVar)
        VirtualProtect(pMemory, game.Scripts.Params[1].nVar, PAGE_EXECUTE_READWRITE, &flOldProtect);
    switch(game.Scripts.Params[1].nVar)
    {
    case 1:
        game.Scripts.Params[0].nVar = *(unsigned char *)pMemory;
        break;
    case 2:
        game.Scripts.Params[0].nVar = *(unsigned short *)pMemory;
        break;
    case 4:
        game.Scripts.Params[0].nVar = *(int *)pMemory;
        break;
    default:
        game.Scripts.Params[0].nVar = 0;
        break;
    }
    if(game.Scripts.Params[2].nVar)
        VirtualProtect(pMemory, game.Scripts.Params[1].nVar, flOldProtect, &flOldProtect);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::CALL(CScript *script)
{
    script->Collect(3);
    void *func = game.Scripts.Params[0].pVar;
    unsigned int popsize = game.Scripts.Params[2].nVar * 4;
    for(int i = 0; i < game.Scripts.Params[1].nVar; i++)
    {
        script->Collect(1);
        unsigned int param = game.Scripts.Params[0].nVar;
        __asm push param
    }
    __asm call func
    __asm add esp, popsize
    while((*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type)
        script->Collect(1);
    script->m_dwIp++;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::CALL_FUNCTION(CScript *script)
{
    script->Collect(3);
    void *func = game.Scripts.Params[0].pVar;
    int func_result;
    unsigned int popsize = game.Scripts.Params[2].nVar * 4;
    for(int i = 0; i < game.Scripts.Params[1].nVar; i++)
    {
        script->Collect(1);
        unsigned int param = game.Scripts.Params[0].nVar;
        __asm push param
    }
    __asm call func
    __asm mov func_result, eax
    __asm add esp, popsize
    game.Scripts.Params[0].nVar = func_result;
    script->Store(1);
    while((*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type)
        script->Collect(1);
    script->m_dwIp++;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::CALL_METHOD(CScript *script)
{
    script->Collect(4);
    void *func = game.Scripts.Params[0].pVar;
    void *object = game.Scripts.Params[1].pVar;
    unsigned int popsize = game.Scripts.Params[3].nVar * 4;
    for(int i = 0; i < game.Scripts.Params[2].nVar; i++)
    {
        script->Collect(1);
        unsigned int param = game.Scripts.Params[0].nVar;
        __asm push param
    }
    __asm mov ecx, object
    __asm call func
    __asm add esp, popsize
    while((*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type)
        script->Collect(1);
    script->m_dwIp++;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::CALL_FUNCTION_METHOD(CScript *script)
{
    script->Collect(4);
    void *func = game.Scripts.Params[0].pVar;
    void *object = game.Scripts.Params[1].pVar;
    int func_result;
    unsigned int popsize = game.Scripts.Params[3].nVar * 4;
    for(int i = 0; i < game.Scripts.Params[2].nVar; i++)
    {
        script->Collect(1);
        unsigned int param = game.Scripts.Params[0].nVar;
        __asm push param
    }
    __asm mov ecx, object
    __asm call func
    __asm mov func_result, eax
    __asm add esp, popsize
    game.Scripts.Params[0].nVar = func_result;
    script->Store(1);
    while((*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type)
        script->Collect(1);
    script->m_dwIp++;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_GAME_VERSION(CScript *script)
{
    game.Scripts.Params[0].nVar = game.Version - 1;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_CHAR_STRUCT(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].pVar = game.Pools.pfPedPoolGetStruct(*game.Pools.pPedPool, game.Scripts.Params[0].nVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_CAR_STRUCT(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].pVar = game.Pools.pfVehiclePoolGetStruct(*game.Pools.pVehiclePool, game.Scripts.Params[0].nVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_OBJECT_STRUCT(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].pVar = game.Pools.pfObjectPoolGetStruct(*game.Pools.pObjectPool, game.Scripts.Params[0].nVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_CHAR_HANDLE(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].nVar = game.Pools.pfPedPoolGetHandle(*game.Pools.pPedPool, game.Scripts.Params[0].pVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_CAR_HANDLE(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].nVar = game.Pools.pfVehiclePoolGetHandle(*game.Pools.pVehiclePool, game.Scripts.Params[0].pVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_OBJECT_HANDLE(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].nVar = game.Pools.pfObjectPoolGetHandle(*game.Pools.pObjectPool, game.Scripts.Params[0].pVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_THREAD_POINTER(CScript *script)
{
    game.Scripts.Params[0].pVar = script;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_NAMED_THREAD_POINTER(CScript *script)
{
    char name[8];
    script->ReadShortString(name);
    CScript *result_ptr = 0;
    CScript *search = scriptMgr.pCusomScripts;
    while(search)
    {
        if(!stricmp(search->m_acName, name))
        {
            result_ptr = search;
            break;
        }
        search = search->m_pNextCustom;
    }
    if(!result_ptr)
    {
        for(int i = 0; i < 128; i++)
        {
            if(!stricmp(scriptMgr.gameScripts[i].m_acName, name))
            {
                result_ptr = &scriptMgr.gameScripts[i];
                break;
            }
        }
    }
    game.Scripts.Params[0].pVar = result_ptr;
    if(result_ptr)
        script->UpdateCompareFlag(true);
    else
        script->UpdateCompareFlag(false);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::IS_KEY_PRESSED(CScript *script)
{
    script->Collect(1);
    script->UpdateCompareFlag(GetKeyState(game.Scripts.Params[0].nVar) & 0x8000);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::FIND_RANDOM_CHAR(CScript *script)
{
    script->Collect(6);
    unsigned int objcount;
    if(game.Scripts.Params[4].nVar)
    {
        if(!script->m_nLastPedSearchIndex)
        {
            game.Scripts.Params[0].nVar = 0;
            script->Store(1);
            script->UpdateCompareFlag(false);
            return OR_CONTINUE;
        }
        objcount = script->m_nLastPedSearchIndex;
    }
    else
    {
        if(!(*game.Pools.pPedPool)->capacity)
        {
            game.Scripts.Params[0].nVar = 0;
            script->Store(1);
            script->UpdateCompareFlag(false);
            return OR_CONTINUE;
        }
        objcount = (*game.Pools.pPedPool)->capacity;
    }
    unsigned int poolobj = (unsigned int)(*game.Pools.pPedPool)->objects + objcount * 0x5F0;
    bool found = 0;
    float maxsq = game.Scripts.Params[3].fVar * game.Scripts.Params[3].fVar;
    for(int i = objcount - 1; i >= 0; i--)
    {
        poolobj -= 0x5F0;
        if(!((*game.Pools.pPedPool)->flags[i] & 0x80))
        {
            if(*(unsigned int *)(poolobj + 0x32C))
            {
                if(!game.Scripts.Params[5].nVar || (*(unsigned int *)(poolobj + 0x224) != 48 && *(unsigned int *)(poolobj + 0x224) != 49))
                {
                    float xd = *(float *)(poolobj + 0x34) - game.Scripts.Params[0].fVar;
                    float yd = *(float *)(poolobj + 0x38) - game.Scripts.Params[1].fVar;
                    float zd = *(float *)(poolobj + 0x3C) - game.Scripts.Params[2].fVar;
                    float distsq = xd * xd + yd * yd + zd * zd;
                    if(distsq <= maxsq)
                    {
                        found = true;
                        script->m_nLastPedSearchIndex = i;
                        game.Scripts.Params[0].nVar = game.Pools.pfPedPoolGetHandle(*game.Pools.pPedPool, (void *)poolobj);
                        script->UpdateCompareFlag(true);
                        break;
                    }
                }
            }
        }
    }
    if(!found)
    {
        game.Scripts.Params[0].nVar = -1;
        script->UpdateCompareFlag(false);
        script->m_nLastPedSearchIndex = 0;
    }
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::FIND_RANDOM_CAR(CScript *script)
{
    script->Collect(6);
    unsigned int objcount;
    if(game.Scripts.Params[4].nVar)
    {
        if(!script->m_nLastVehicleSearchIndex)
        {
            game.Scripts.Params[0].nVar = 0;
            script->Store(1);
            script->UpdateCompareFlag(false);
            return OR_CONTINUE;
        }
        objcount = script->m_nLastVehicleSearchIndex;
    }
    else
    {
        if(!(*game.Pools.pVehiclePool)->capacity)
        {
            game.Scripts.Params[0].nVar = 0;
            script->Store(1);
            script->UpdateCompareFlag(false);
            return OR_CONTINUE;
        }
        objcount = (*game.Pools.pVehiclePool)->capacity;
    }
    unsigned int poolobj = (unsigned int)(*game.Pools.pVehiclePool)->objects + objcount * 0x5A8;
    bool found = 0;
    float maxsq = game.Scripts.Params[3].fVar * game.Scripts.Params[3].fVar;
    for(int i = objcount - 1; i >= 0; i--)
    {
        poolobj -= 0x5A8;
        if(!((*game.Pools.pVehiclePool)->flags[i] & 0x80))
        {
            if(!game.Scripts.Params[5].nVar || ((*(unsigned char *)(poolobj + 0x50) & 0xF8) != 40 && *(unsigned int *)(poolobj + 0x284) != 1 && !(*(unsigned char *)(poolobj + 0x122) & 8)))
            {
                float xd = *(float *)(poolobj + 0x34) - game.Scripts.Params[0].fVar;
                float yd = *(float *)(poolobj + 0x38) - game.Scripts.Params[1].fVar;
                float zd = *(float *)(poolobj + 0x3C) - game.Scripts.Params[2].fVar;
                float distsq = xd * xd + yd * yd + zd * zd;
                if(distsq <= maxsq)
                {
                    found = true;
                    script->m_nLastVehicleSearchIndex = i;
                    game.Scripts.Params[0].nVar = game.Pools.pfVehiclePoolGetHandle(*game.Pools.pVehiclePool, (void *)poolobj);
                    script->UpdateCompareFlag(true);
                    break;
                }
            }
        }
    }
    if(!found)
    {
        game.Scripts.Params[0].nVar = -1;
        script->UpdateCompareFlag(false);
        script->m_nLastVehicleSearchIndex = 0;
    }
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::FIND_RANDOM_OBJECT(CScript *script)
{
    script->Collect(5);
    unsigned int objcount;
    if(game.Scripts.Params[4].nVar)
    {
        if(!script->m_nLastObjectSearchIndex)
        {
            game.Scripts.Params[0].nVar = 0;
            script->Store(1);
            script->UpdateCompareFlag(false);
            return OR_CONTINUE;
        }
        objcount = script->m_nLastObjectSearchIndex;
    }
    else
    {
        if(!(*game.Pools.pObjectPool)->capacity)
        {
            game.Scripts.Params[0].nVar = 0;
            script->Store(1);
            script->UpdateCompareFlag(false);
            return OR_CONTINUE;
        }
        objcount = (*game.Pools.pObjectPool)->capacity;
    }
    unsigned int poolobj = (unsigned int)(*game.Pools.pObjectPool)->objects + objcount * 0x19C;
    bool found = 0;
    float maxsq = game.Scripts.Params[3].fVar * game.Scripts.Params[3].fVar;
    for(int i = objcount - 1; i >= 0; i--)
    {
        poolobj -= 0x19C;
        if(!((*game.Pools.pObjectPool)->flags[i] & 0x80))
        {
            float xd = *(float *)(poolobj + 0x34) - game.Scripts.Params[0].fVar;
            float yd = *(float *)(poolobj + 0x38) - game.Scripts.Params[1].fVar;
            float zd = *(float *)(poolobj + 0x3C) - game.Scripts.Params[2].fVar;
            float distsq = xd * xd + yd * yd + zd * zd;
            if(distsq <= maxsq)
            {
                found = true;
                script->m_nLastObjectSearchIndex = i;
                game.Scripts.Params[0].nVar = game.Pools.pfObjectPoolGetHandle(*game.Pools.pObjectPool, (void *)poolobj);
                script->UpdateCompareFlag(true);
                break;
            }
        }
    }
    if(!found)
    {
        game.Scripts.Params[0].nVar = -1;
        script->UpdateCompareFlag(false);
        script->m_nLastObjectSearchIndex = 0;
    }
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::CALL_POP_FLOAT(CScript *script)
{
    float *pParam = &game.Scripts.Params[0].fVar;
    __asm mov eax, pParam
    __asm fstp [eax]
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::MATH_EXP(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].fVar = powf(game.Scripts.Params[0].fVar, game.Scripts.Params[1].fVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::MATH_LOG(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].fVar =  logf(game.Scripts.Params[0].fVar) / logf(game.Scripts.Params[1].fVar);
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::CALL_SCM_FUNCTION(CScript *script)
{
    script->m_pScmFunction = new ScmFunction(script);
    script->Collect(2);
    int addr = game.Scripts.Params[0].nVar;
    unsigned int paramCount = game.Scripts.Params[1].nVar;
    if(paramCount > 0)
        script->Collect(paramCount);
    memset(script->m_aLVars, 0, 64);
    if(paramCount > 0)
        memcpy(script->m_aLVars, game.Scripts.Params, paramCount * 4);
    script->m_pScmFunction->retAddr = script->m_dwIp;
    script->JumpTo(addr);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::SCM_FUNCTION_RET(CScript *script)
{
    script->Collect(1);
    unsigned int paramCount = game.Scripts.Params[0].nVar;
    if(paramCount > 0)
        script->Collect(paramCount);
    memcpy(script->m_aLVars, script->m_pScmFunction->vars, 64);
    script->m_dwIp = script->m_pScmFunction->retAddr;
    if(paramCount > 0)
        script->Store(paramCount);
    script->m_dwIp++;
    ScmFunction *prev = script->m_pScmFunction->prev;
    delete script->m_pScmFunction;
    script->m_pScmFunction = prev;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_LABEL_OFFSET(CScript *script)
{
    script->Collect(1);
    if(game.Scripts.Params[0].nVar >= 0)
        game.Scripts.Params[0].pVar = &game.Scripts.Space[game.Scripts.Params[0].nVar];
    else
    {
        if(script->m_nScriptType == SCRIPT_TYPE_CUSTOM)
            game.Scripts.Params[0].pVar = &script->m_pCodeData[-game.Scripts.Params[0].nVar];
        else
            game.Scripts.Params[0].pVar = &game.Scripts.Space[0x370E8 - game.Scripts.Params[0].nVar];
    }
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::GET_VAR_OFFSET(CScript *script)
{
    game.Scripts.Params[0].pVar = script->GetPointerToScriptVariable();
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_AND(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar & game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_OR(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar | game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_XOR(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar ^ game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_NOT(CScript *script)
{
    script->Collect(1);
    game.Scripts.Params[0].nVar = ~game.Scripts.Params[0].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_MOD(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar % game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_SHR(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar >> game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::BIT_SHL(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar << game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::START_CUSTOM_THREAD_VSTRING(CScript *script)
{
    script->Collect(1);
    char filepath[MAX_PATH];
    strcpy(filepath, "cleo\\");
    strcat(filepath, game.Scripts.Params[0].cVar);
    CScript *newScript = new CScript(filepath);
    if(newScript && newScript->Loaded())
    {
        newScript->AddToCustomList(&scriptMgr.pCusomScripts);
        scriptMgr.numLoadedCustomScripts++;
        game.Scripts.AddScriptToList(newScript, game.Scripts.pActiveScriptsList);
        newScript->m_bIsActive = true;
        LOGL(LOG_PRIORITY_OPCODE, "START_CUSTOM_THREAD: Started new script \"%s\"", script->m_acName);
        for(int i = 0; script->GetNextParamType(); i++)
        {
            script->Collect(1);
            newScript->m_aLVars[i].nVar = game.Scripts.Params[0].nVar;
        }
        script->UpdateCompareFlag(true);
    }
    else
    {
        while(script->GetNextParamType())
            script->Collect(1);
        LOGL(LOG_PRIORITY_OPCODE, "START_CUSTOM_THREAD: Script loading failed, \"%s\"", script->m_acName);
        if(newScript)
            delete newScript;
        script->UpdateCompareFlag(false);
    }
    script->m_dwIp++;
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::DRAW_SHADOW(CScript *script)
{
    script->Collect(10);
    int Type = 2;	
    CVector pos;
    pos.x = game.Scripts.Params[1].fVar;
    pos.y = game.Scripts.Params[2].fVar;
    pos.z = game.Scripts.Params[3].fVar;
    //float Angle = game.Scripts.Params[4].fVar; // to do
    float Size = game.Scripts.Params[5].fVar;

    switch (game.Scripts.Params[0].nVar)
    {
    case 1u:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_shad_car;
        Type = 1;
        break;
    case 2u:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_shad_ped;
        break;
    case 3u:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_shad_exp;
        break;
    case 4:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_shad_heli;
        Type = 1;
        break;
    case 5:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_headlight;
        break;
    case 6:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_bloodpool_64;
        break;
    case 7:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_shad_bike;
        break;
    case 8:
        game.Shadows.pRwTexture = game.Shadows.pRwTexture_shad_rcbaron;
        break;
    case 0u:
        return OR_CONTINUE;
    }

    game.Shadows.StoreShadowToBeRendered(Type, *game.Shadows.pRwTexture, &pos, Size, 0.0, 0.0, -Size, game.Scripts.Params[6].nVar, game.Scripts.Params[7].nVar, game.Scripts.Params[8].nVar, game.Scripts.Params[9].nVar, 150.0f, true, 1.0f, false, false);
    return OR_CONTINUE;
}

eOpcodeResult CustomOpcodes::SET_TEXT_DRAW_FONT(CScript *script)
{
    script->Collect(1);
    game.Text.textDrawers[*game.Text.currentTextDrawer].fontStyle = game.Scripts.Params[0].nVar;
    return OR_CONTINUE;
}

//0A8E=3,%3d% = %1d% + %2d% ; int
eOpcodeResult CustomOpcodes::OPCODE_0A8E(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar + game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

//0A8F=3,%3d% = %1d% - %2d% ; int
eOpcodeResult CustomOpcodes::OPCODE_0A8F(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar - game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

//0A90=3,%3d% = %1d% * %2d% ; int
eOpcodeResult CustomOpcodes::OPCODE_0A90(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar * game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

//0A91=3,%3d% = %1d% / %2d% ; int
eOpcodeResult CustomOpcodes::OPCODE_0A91(CScript *script)
{
    script->Collect(2);
    game.Scripts.Params[0].nVar = game.Scripts.Params[0].nVar / game.Scripts.Params[1].nVar;
    script->Store(1);
    return OR_CONTINUE;
}

//0ADC=1, test_cheat %1d%
eOpcodeResult CustomOpcodes::OPCODE_0ADC(CScript *script)
{
    script->Collect(1);

    char *c = game.Text.cheatString;
    char buf[30];
    strcpy(buf, game.Scripts.Params[0].cVar);
    char *s = _strrev(buf);
    while (*s && toupper(*s++) == *c++);
    if (*s)
    {
        script->UpdateCompareFlag(false);
        return OR_CONTINUE;
    }
    game.Text.cheatString[0] = 0;
    script->UpdateCompareFlag(true);

    return OR_CONTINUE;
}

//0ADF=2,add_dynamic_GXT_entry %1d% text %2d%
eOpcodeResult CustomOpcodes::OPCODE_0ADF(CScript *script)
{
    script->Collect(2);

    CustomTextEntry *entry = new CustomTextEntry(game.Scripts.Params[0].cVar, game.Scripts.Params[1].cVar);
    if (entry)
    {
        entry->m_pNext = CustomText::pCustomTextList;
        CustomText::pCustomTextList = entry;
    }

    return OR_CONTINUE;
}

//0AE0 = 1, remove_dynamic_GXT_entry %1d%
eOpcodeResult CustomOpcodes::OPCODE_0AE0(CScript *script)
{
    script->Collect(1);

    CustomTextEntry *entry = CustomText::pCustomTextList;
    while (entry)
    {
        CustomTextEntry *next = entry->m_pNext;
		if (next)
		{
			if (strcmp(game.Scripts.Params[0].cVar, next->m_key) == 0)
			{
				next->m_pNext = next->m_pNext->m_pNext;
				LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Unloaded custom text \"%s\"", entry->m_key);
				delete entry;
				return OR_CONTINUE;
			}
			else
			{
				CustomText::pCustomTextList = entry->m_pNext;
				LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Unloaded custom text \"%s\"", entry->m_key);
				delete entry;
				return OR_CONTINUE;
			}
		}
		else
		{
			LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Unloaded custom text \"%s\"", entry->m_key);
			delete entry;
			CustomText::pCustomTextList = 0;
			return OR_CONTINUE;
		}
		entry = next;
    }

    return OR_CONTINUE;
}

//0AA2=2,%2h% = load_library %1d% // IF and SET
eOpcodeResult CustomOpcodes::OPCODE_0AA2(CScript *script)
{
	script->Collect(1);
	auto libHandle = LoadLibrary(game.Scripts.Params[0].cVar);
	game.Scripts.Params[0].pVar = libHandle;
	script->Store(1);
	script->UpdateCompareFlag(libHandle != nullptr);
	return OR_CONTINUE;
}

//0AA3=1,free_library %1h%
eOpcodeResult CustomOpcodes::OPCODE_0AA3(CScript *script)
{
	script->Collect(1);
	HMODULE libHandle;
	libHandle = (HMODULE)game.Scripts.Params[0].pVar;
	FreeLibrary(libHandle);
	return OR_CONTINUE;
}

//0AA4=3,%3d% = get_proc_address %1d% library %2d% // IF and SET
eOpcodeResult CustomOpcodes::OPCODE_0AA4(CScript *script)
{
	script->Collect(2);
	char *funcName = game.Scripts.Params[0].cVar;
	HMODULE libHandle;
	libHandle = (HMODULE)game.Scripts.Params[1].pVar;
	void *funcAddr = (void *)GetProcAddress(libHandle, funcName);
	game.Scripts.Params[0].pVar = funcAddr;
	script->Store(1);
	script->UpdateCompareFlag(funcAddr != nullptr);
	return OR_CONTINUE;
}

//0AAB=1,  file_exists %1d%
eOpcodeResult CustomOpcodes::OPCODE_0AAB(CScript *script)
{
	script->Collect(1);
	DWORD fAttr = GetFileAttributes(game.Scripts.Params[0].cVar);
	script->UpdateCompareFlag((fAttr != INVALID_FILE_ATTRIBUTES) &&
		!(fAttr & FILE_ATTRIBUTE_DIRECTORY));
	return OR_CONTINUE;
}

wchar_t message_buf[0x80];
//0ACA=1,show_text_box %1d%
eOpcodeResult CustomOpcodes::OPCODE_0ACA(CScript *script)
{
	script->Collect(1);
	swprintf(message_buf, 100, L"%hs", game.Scripts.Params[0].cVar);
	game.Text.TextBox(message_buf, false, false, false);
	return OR_CONTINUE;
};

//0ACB=3,show_styled_text %1d% time %2d% style %3d%
eOpcodeResult CustomOpcodes::OPCODE_0ACB(CScript *script)
{
	script->Collect(3);
	const char *text = game.Scripts.Params[0].cVar;
	unsigned time, style;
	swprintf(message_buf, 100, L"%hs", text);
	game.Text.StyledText(message_buf, game.Scripts.Params[1].nVar, game.Scripts.Params[2].nVar - 1);
	return OR_CONTINUE;
};

//0ACC=2,show_text_lowpriority %1d% time %2d%
eOpcodeResult CustomOpcodes::OPCODE_0ACC(CScript *script)
{
	script->Collect(2);
	const char *text = game.Scripts.Params[0].cVar;
	swprintf(message_buf, 100, L"%hs", text);
	game.Text.TextLowPriority(message_buf, game.Scripts.Params[1].nVar, false, false);
	return OR_CONTINUE;
};

//0ACD=2,show_text_highpriority %1d% time %2d%
eOpcodeResult CustomOpcodes::OPCODE_0ACD(CScript *script)
{
	script->Collect(2);
	const char *text = game.Scripts.Params[0].cVar;
	swprintf(message_buf, 100, L"%hs", text);
	game.Text.TextHighPriority(message_buf, game.Scripts.Params[1].nVar, false, false);
	return OR_CONTINUE;
};

/*//0ACE=-1,show_formatted_text_box %1d%
eOpcodeResult CustomOpcodes::OPCODE_0ACE(CScript *script)
{
	char fmt[100]; char text[100];
	readString(thread, fmt, sizeof(fmt));
	format(thread, text, sizeof(text), fmt);
	ShowTextBox(text);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
};

//0ACF=-1,show_formatted_styled_text %1d% time %2d% style %3d%
eOpcodeResult CustomOpcodes::OPCODE_0ACF(CScript *script)
{
	char fmt[100]; char text[100];
	unsigned time, style;
	readString(thread, fmt, sizeof(fmt));
	RetrieveScriptParams(thread, time, style);
	format(thread, text, sizeof(text), fmt);
	ShowStyledText(text, time, style);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
};

//0AD0=-1,show_formatted_text_lowpriority %1d% time %2d%
eOpcodeResult CustomOpcodes::OPCODE_0AD0(CScript *script)
{
	char fmt[100]; char text[100];
	unsigned time;
	readString(thread, fmt, sizeof(fmt));
	*thread >> time;
	format(thread, text, sizeof(text), fmt);
	ShowTextLowPriority(text, time);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
};

//0AD1=-1,show_formatted_text_highpriority %1d% time %2d%
eOpcodeResult CustomOpcodes::OPCODE_0AD1(CScript *script)
{
	char fmt[100]; char text[100];
	unsigned time;
	readString(thread, fmt, sizeof(fmt));
	*thread >> time;
	format(thread, text, sizeof(text), fmt);
	ShowTextHighPriority(text, time);
	SkipUnusedParameters(thread);
	return OR_CONTINUE;
};*/

////////////////////////////////////////////INI FILES///////////////////////////
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

//0AF0=4,%4d% = get_int_from_ini_file %1s% section %2s% key %3s%
eOpcodeResult CustomOpcodes::OPCODE_0AF0(CScript *script)
{
    script->Collect(3);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    int result;

    strcpy(path, game.Scripts.Params[0].cVar);
    strcpy(sectionName, game.Scripts.Params[1].cVar);
    strcpy(key, game.Scripts.Params[2].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = GetPrivateProfileInt(sectionName, key, 0x80000000, iniPath);
    game.Scripts.Params[0].nVar = result;
    script->Store(1);
    script->UpdateCompareFlag(result != 0x80000000);

    return OR_CONTINUE;
}

//0AF1=4,write_int %1d% to_ini_file %2s% section %3s% key %4s%
eOpcodeResult CustomOpcodes::OPCODE_0AF1(CScript *script)
{
    script->Collect(4);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    DWORD value;
    char strValue[100];
    BOOL result;

    value = game.Scripts.Params[0].nVar;
    strcpy(path, game.Scripts.Params[1].cVar);
    strcpy(sectionName, game.Scripts.Params[2].cVar);
    strcpy(key, game.Scripts.Params[3].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = WritePrivateProfileString(sectionName, key, _itoa(value, strValue, 10), iniPath);
    script->UpdateCompareFlag(result != 0);

    return OR_CONTINUE;
}

//0AF2=4,%4d% = get_float_from_ini_file %1s% section %2s% key %3s%
eOpcodeResult CustomOpcodes::OPCODE_0AF2(CScript *script)
{
    script->Collect(3);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    float value = 0.0f;
    char strValue[100];
    BOOL result;

    strcpy(path, game.Scripts.Params[0].cVar);
    strcpy(sectionName, game.Scripts.Params[1].cVar);
    strcpy(key, game.Scripts.Params[2].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);
    if (result)
    {
        value = (float)atof(strValue);
        game.Scripts.Params[0].fVar = value;
        script->Store(1);
    }
    else
    {
        game.Scripts.Params[0].fVar = 0.0f;
        script->Store(1);
    }

    script->UpdateCompareFlag(result != 0);

    return OR_CONTINUE;
}

//0AF3=4,write_float %1d% to_ini_file %2s% section %3s% key %4s%
eOpcodeResult CustomOpcodes::OPCODE_0AF3(CScript *script)
{
    script->Collect(4);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    float value;
    char strValue[100];
    BOOL result;

    value = game.Scripts.Params[0].fVar;
    strcpy(path, game.Scripts.Params[1].cVar);
    strcpy(sectionName, game.Scripts.Params[2].cVar);
    strcpy(key, game.Scripts.Params[3].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    sprintf(strValue, "%g", value);

    result = WritePrivateProfileString(sectionName, key, strValue, iniPath);
    script->UpdateCompareFlag(result != 0);

    return OR_CONTINUE;
}

//0AF4=4,%4d% = read_string_from_ini_file %1s% section %2s% key %3s%
eOpcodeResult CustomOpcodes::OPCODE_0AF4(CScript *script)
{
    script->Collect(3);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    char strValue[100];
    char *strptr;
    BOOL result;

    strcpy(path, game.Scripts.Params[0].cVar);
    strcpy(sectionName, game.Scripts.Params[1].cVar);
    strcpy(key, game.Scripts.Params[2].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = GetPrivateProfileString(sectionName, key, NULL, strValue, sizeof(strValue), iniPath);

    strcpy(game.Scripts.Params[0].cVar, strValue);
    script->Store(1);

    script->UpdateCompareFlag(result != 0);

    return OR_CONTINUE;
}

//0AF5=4,write_string %1s% to_ini_file %2s% section %3s% key %4s%
eOpcodeResult CustomOpcodes::OPCODE_0AF5(CScript *script)
{
    script->Collect(4);

    char iniPath[MAX_PATH];
    char path[100];
    char sectionName[100];
    char key[100];
    char strValue[100];
    char *strptr;
    BOOL result;

    switch ((*(tParamType *)(&game.Scripts.Space[script->m_dwIp])).type)
    {
    case PARAM_TYPE_STRING:
        strcpy(strValue, game.Scripts.Params[0].cVar);
        break;
    default:
        strptr = (char *)game.Scripts.Params[0].nVar;
        strcpy(strValue, strptr);
    }
    strcpy(strValue, game.Scripts.Params[0].cVar);
    strcpy(path, game.Scripts.Params[1].cVar);
    strcpy(sectionName, game.Scripts.Params[2].cVar);
    strcpy(key, game.Scripts.Params[3].cVar);

    //if path is short, GetPrivateProfileInt() searches for the file in the Windows directory
    MakeFullPath(path, iniPath);

    result = WritePrivateProfileString(sectionName, key, strValue, iniPath);

    script->UpdateCompareFlag(result != 0);

    return OR_CONTINUE;
}