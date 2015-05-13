#include "Game.h"
#include "CPatch.h"
#include "ScriptManager.h"
#include "Log.h"
#include "Fxt.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"

GtaGame game;

#define GAME_VERSION_ID (*(unsigned int *)0x61C11C)

#define GAME_ID_GTA3_1_0 0x598B80
#define GAME_ID_GTA3_1_1 0x598E40

GtaGame::GtaGame()
{
	this->InitialiseGameVersion();
	this->InitAndPatch();
}

void GtaGame::InitialiseGameVersion()
{
	switch(GAME_VERSION_ID)
	{
	case GAME_ID_GTA3_1_0:
		this->Version = GAME_V1_0;
		break;
	case GAME_ID_GTA3_1_1:
		this->Version = GAME_V1_1;
		break;
	default:
		this->Version = GAME_UNKNOWN;
		break;
	}
}

eGameVersion GtaGame::GetGameVersion()
{
	return this->Version;
}

void GtaGame::InitAndPatch()
{
	switch(this->Version)
	{
	case GAME_V1_0:
		// Scripts
		CPatch::SetPointer(0x438809, scriptMgr.gameScripts);
		CPatch::SetInt(0x43882A, sizeof(CScript));
		CPatch::RedirectJump(0x4386C0, ScriptManager::InitialiseScript);
		CPatch::RedirectJump(0x439500, ScriptManager::ProcessScriptCommand);
		CPatch::RedirectJump(0x4382E0, ScriptManager::CollectScriptParameters);
		CPatch::RedirectJump(0x438460, ScriptManager::CollectScriptNextParameterWithoutIncreasingPC);
		this->Scripts.AddScriptToList = (void (__thiscall *)(CScript *, CScript **))0x438FE0;
		this->Scripts.RemoveScriptFromList = (void (__thiscall *)(CScript *, CScript **))0x438FB0;
		this->Scripts.StoreParameters = (void (__thiscall *)(CScript *, unsigned int *, unsigned int))0x4385A0;
		this->Scripts.UpdateCompareFlag = (void (__thiscall *)(CScript *, bool))0x44FD90;
		this->Scripts.GepPointerToScriptVariable = (void *(__thiscall *)(CScript *, unsigned int *, unsigned char))0x438640;
		this->Scripts.OpcodeHandlers[0] = (OpcodeHandler)0x439650;
		this->Scripts.OpcodeHandlers[1] = (OpcodeHandler)0x43AEA0;
		this->Scripts.OpcodeHandlers[2] = (OpcodeHandler)0x43D530;
		this->Scripts.OpcodeHandlers[3] = (OpcodeHandler)0x43ED30;
		this->Scripts.OpcodeHandlers[4] = (OpcodeHandler)0x440CB0;
		this->Scripts.OpcodeHandlers[5] = (OpcodeHandler)0x4429C0;
		this->Scripts.OpcodeHandlers[6] = (OpcodeHandler)0x444B20;
		this->Scripts.OpcodeHandlers[7] = (OpcodeHandler)0x4458A0;
		this->Scripts.OpcodeHandlers[8] = (OpcodeHandler)0x448240;
		this->Scripts.OpcodeHandlers[9] = (OpcodeHandler)0x44CB80;
		this->Scripts.OpcodeHandlers[10] = (OpcodeHandler)0x588490;
		this->Scripts.OpcodeHandlers[11] = (OpcodeHandler)0x589D00;
		this->Scripts.pActiveScriptsList = (CScript **)0x8E2BF4;
		this->Scripts.Params = (tScriptVar *)0x6ED460;
		this->Scripts.Space = (char *)0x74B248;
		this->Scripts.pNumOpcodesExecuted = (unsigned short *)0x95CCA6;
		// Text
		this->Text.pfGetText = (wchar_t *(__thiscall *)(int, char *))0x52BFB0;
		CPatch::RedirectJump(0x52C5A0, CustomText::GetText);
		// Screen
		this->Screen.Width = (int *)0x8F436C;
		this->Screen.Height = (int *)0x8F4370;
		// Font
		this->Font.AsciiToUnicode = (void (__cdecl *)(const char *, short *)) 0x5009C0;
		this->Font.PrintString = (void (__cdecl *)(float, float, wchar_t *)) 0x500F50;
		this->Font.SetFontStyle = (void (__cdecl *)(int)) 0x501DB0;
		this->Font.SetScale = (void (__cdecl *)(float, float)) 0x501B80;
		this->Font.SetColor = (void (__cdecl *)(unsigned int *)) 0x501BD0;
		this->Font.SetLeftJustifyOn = (void (__cdecl *)()) 0x501C60;
		this->Font.SetDropShadowPosition = (void (__cdecl *)(int)) 0x501E70;
		this->Font.SetPropOn = (void (__cdecl *)()) 0x501DA0;
		// Pools
		this->Pools.pPedPool = (GamePool **)0x8F2C60;
		this->Pools.pVehiclePool = (GamePool **)0x9430DC;
		this->Pools.pObjectPool = (GamePool **)0x880E28;
		this->Pools.pfPedPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x43EB30;
		this->Pools.pfVehiclePoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x43EAF0;
		this->Pools.pfObjectPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x43EAB0;
		this->Pools.pfPedPoolGetHandle = (int (__thiscall *)(GamePool *, void *))0x43EB70;
		this->Pools.pfVehiclePoolGetHandle = (int (__thiscall *)(GamePool *, void *))0x429050;
		this->Pools.pfObjectPoolGetHandle = (int (__thiscall *)(GamePool *, void *))0x429000;
		// Events
		this->Events.pfInitScripts_OnGameSaveLoad = (void (__cdecl *)())CPatch::MakeCallAddr(0x453B43, 0x438790);
		CPatch::RedirectCall(0x453B43, GtaGame::InitScripts_OnGameSaveLoad);
		this->Events.pfInitScripts_OnGameInit = (void (__cdecl *)())CPatch::MakeCallAddr(0x48C26B, 0x438790);
		CPatch::RedirectCall(0x48C26B, GtaGame::InitScripts_OnGameInit);
		this->Events.pfInitScripts_OnGameReinit = (void (__cdecl *)())CPatch::MakeCallAddr(0x48C575, 0x438790);
		CPatch::RedirectCall(0x48C575, GtaGame::InitScripts_OnGameReinit);
		this->Events.pfShutdownGame = (void (__cdecl *)())CPatch::MakeCallAddr(0x48C4A2, 0x406300);
		CPatch::RedirectCall(0x48C4A2, GtaGame::OnShutdownGame);
		this->Events.pfGameSaveScripts = (void (__cdecl *)(int, int))CPatch::MakeCallAddr(0x58FBD9, 0x4535E0);
		CPatch::RedirectCall(0x58FBD9, GtaGame::OnGameSaveScripts);
		this->Events.pfDrawInMenu = (void (__cdecl *)(float, float, wchar_t *))CPatch::MakeCallAddr(0x47AF76, 0x500F50);
		CPatch::RedirectCall(0x47AF76, GtaGame::OnMenuDrawing);
		break;
	case GAME_V1_1:
		break;
	default:
		break;
	}
}

void GtaGame::InitScripts_OnGameInit()
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Init--");
	scriptMgr.UnloadScripts();
	CustomText::Unload();
	game.Events.pfInitScripts_OnGameInit();
	scriptMgr.LoadScripts();
	CustomText::Load();
}

void GtaGame::InitScripts_OnGameReinit()
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Re-Init--");
	scriptMgr.UnloadScripts();
	CustomText::Unload();
	game.Events.pfInitScripts_OnGameReinit();
	scriptMgr.LoadScripts();
	CustomText::Load();
}

void GtaGame::InitScripts_OnGameSaveLoad()
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Load Save--");
	scriptMgr.UnloadScripts();
	CustomText::Unload();
	game.Events.pfInitScripts_OnGameSaveLoad();
	scriptMgr.LoadScripts();
	CustomText::Load();
}

void GtaGame::OnShutdownGame()
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Shutdown--");
	game.Events.pfShutdownGame();
	scriptMgr.UnloadScripts();
	CustomText::Unload();
}

void GtaGame::OnGameSaveScripts(int a, int b)
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Save Scripts--");
	scriptMgr.DisableAllScripts();
	game.Events.pfGameSaveScripts(a, b);
	scriptMgr.EnableAllScripts();
}

float ScreenCoord(float a)
{
	return a*(((float)(*game.Screen.Height))/900.f);
}

void GtaGame::OnMenuDrawing(float x, float y, wchar_t *text)
{
	game.Events.pfDrawInMenu(x, y, text);
	unsigned char color[4] = {0xEB, 0xAA, 0x32, 0xFF};
	game.Font.SetColor((unsigned int *)color);
	game.Font.SetDropShadowPosition(0);
	game.Font.SetPropOn();
	game.Font.SetFontStyle(0);
	game.Font.SetScale(ScreenCoord(0.45f), ScreenCoord(0.7f));
	game.Font.SetLeftJustifyOn();
	wchar_t line[128];
	swprintf(line, L"CLEO v%d.%d.%d.%d", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);
	game.Font.PrintString(ScreenCoord(30.0f), (float)*game.Screen.Height - ScreenCoord(34.0f), line);
	swprintf(line, L"%d %s, %d %s loaded", scriptMgr.numLoadedCustomScripts, scriptMgr.numLoadedCustomScripts == 1? L"script" : L"scripts",
		CleoPlugins::numLoadedPlugins, CleoPlugins::numLoadedPlugins == 1? L"plugin" : L"plugins");
	game.Font.PrintString(ScreenCoord(30.0f), (float)*game.Screen.Height - ScreenCoord(20.0f), line);
}