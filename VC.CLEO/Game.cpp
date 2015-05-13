#include "Game.h"
#include "CPatch.h"
#include "ScriptManager.h"
#include "Log.h"
#include "Fxt.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"

GtaGame game;

#define GAME_VERSION_ID (*(unsigned int *)0x61C11C)

#define GAME_ID_GTAVC_1_0 0x74FF5064
#define GAME_ID_GTAVC_1_1 0xFFFFFFF

GtaGame::GtaGame()
{
	this->InitialiseGameVersion();
	this->InitAndPatch();
}

void GtaGame::InitialiseGameVersion()
{
	switch(GAME_VERSION_ID)
	{
	case GAME_ID_GTAVC_1_0:
		this->Version = GAME_V1_0;
		break;
	case GAME_ID_GTAVC_1_1:
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
		CPatch::SetPointer(0x4504E4, scriptMgr.gameScripts); //vc
		CPatch::SetPointer(0x450508, scriptMgr.gameScripts); //vc
		CPatch::SetInt(0x450527 + 2, sizeof(CScript)); //vc
		CPatch::SetInt(0x45052D + 2, sizeof(CScript)); //vc
		CPatch::RedirectJump(0x450CF0, ScriptManager::InitialiseScript); //vc
		CPatch::RedirectJump(0x44FBE0, ScriptManager::ProcessScriptCommand); //vc
		CPatch::RedirectJump(0x451010, ScriptManager::CollectScriptParameters); //vc
		CPatch::RedirectJump(0x450EF0, ScriptManager::CollectScriptNextParameterWithoutIncreasingPC); //vc
		this->Scripts.AddScriptToList = (void(__thiscall *)(CScript *, CScript **))0x4502E0; //vc
		this->Scripts.RemoveScriptFromList = (void(__thiscall *)(CScript *, CScript **))0x450300; //vc
		this->Scripts.StoreParameters = (void(__thiscall *)(CScript *, unsigned int *, unsigned int))0x450E50; //vc
		this->Scripts.UpdateCompareFlag = (void(__thiscall *)(CScript *, bool))0x463F00;
		this->Scripts.GepPointerToScriptVariable = (void *(__thiscall *)(CScript *, unsigned int *, unsigned char))0x450DD0;//vc
		this->Scripts.OpcodeHandlers[0] = (OpcodeHandler)0x44B400; //vc
		this->Scripts.OpcodeHandlers[1] = (OpcodeHandler)0x446390; //vc
		this->Scripts.OpcodeHandlers[2] = (OpcodeHandler)0x444BE0; //vc
		this->Scripts.OpcodeHandlers[3] = (OpcodeHandler)0x453670; //vc
		this->Scripts.OpcodeHandlers[4] = (OpcodeHandler)0x451F90; //vc
		this->Scripts.OpcodeHandlers[5] = (OpcodeHandler)0x457580; //vc
		this->Scripts.OpcodeHandlers[6] = (OpcodeHandler)0x456E20; //vc
		this->Scripts.OpcodeHandlers[7] = (OpcodeHandler)0x455030; //vc
		this->Scripts.OpcodeHandlers[8] = (OpcodeHandler)0x45B220; //vc
		this->Scripts.OpcodeHandlers[9] = (OpcodeHandler)0x458EC0; //vc
		this->Scripts.OpcodeHandlers[10] = (OpcodeHandler)0x6084C0; //vc
		this->Scripts.OpcodeHandlers[11] = (OpcodeHandler)0x606730; //vc
		this->Scripts.OpcodeHandlers[12] = (OpcodeHandler)0x630650; //vc
		this->Scripts.OpcodeHandlers[13] = (OpcodeHandler)0x62E940; //vc
		this->Scripts.OpcodeHandlers[14] = (OpcodeHandler)0x637600; //vc
		this->Scripts.pActiveScriptsList = (CScript **)0x975338; //vc
		this->Scripts.Params = (tScriptVar *)0x7D7438; //vc
		this->Scripts.Space = (char *)0x821280; //vc
		this->Scripts.pNumOpcodesExecuted = (unsigned short *)0xA10A66; //vc
		// Text
		this->Text.pfGetText = (wchar_t *(__thiscall *)(int, char *))0x584F30; //vc
		// Screen
		this->Screen.Width = (int *)0x9B48E4; //vc
		this->Screen.Height = (int *)0x9B48E8; //vc
		// Font
		this->Font.AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x552500; //vc
		this->Font.PrintString = (void(__cdecl *)(float, float, wchar_t *)) 0x551040; //vc
		this->Font.SetFontStyle = (void(__cdecl *)(int)) 0x54FFE0; //vc
		this->Font.SetScale = (void(__cdecl *)(float, float)) 0x550230; //vc
		this->Font.SetColor = (void(__cdecl *)(unsigned int *)) 0x550170; //vc
		this->Font.SetLeftJustifyOn = (void(__cdecl *)()) 0x550120; //vc
		this->Font.SetDropShadowPosition = (void(__cdecl *)(int)) 0x54FF20; //vc
		this->Font.SetPropOn = (void(__cdecl *)()) 0x550020; // vc
		// Pools
		this->Pools.pPedPool = (GamePool **)0x97F2AC; //vc
		this->Pools.pVehiclePool = (GamePool **)0xA0FDE4; //vc
		this->Pools.pObjectPool = (GamePool **)0x94DBE0; //vc
		this->Pools.pfPedPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451CB0; //vc
		this->Pools.pfVehiclePoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451C70; //vc
		this->Pools.pfObjectPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451C30; //vc
		this->Pools.pfPedPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x451CF0; //vc
		this->Pools.pfVehiclePoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x42C4B0; //vc
		this->Pools.pfObjectPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x434A10; //vc
		// Events
		this->Events.pfInitScripts_OnGameSaveLoad = (void(__cdecl *)())CPatch::MakeCallAddr(0x45F463, 0x450330); //vc
		CPatch::RedirectCall(0x45F463, GtaGame::InitScripts_OnGameSaveLoad); //vc
		this->Events.pfInitScripts_OnGameInit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A492F, 0x450330);
		CPatch::RedirectCall(0x4A492F, GtaGame::InitScripts_OnGameInit); //vc
		this->Events.pfInitScripts_OnGameReinit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4E96, 0x450330);
		CPatch::RedirectCall(0x4A4E96, GtaGame::InitScripts_OnGameReinit); //vc
		this->Events.pfShutdownGame = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4AFF, 0x408150);
		CPatch::RedirectCall(0x4A4AFF, GtaGame::OnShutdownGame); //vc
		this->Events.pfGameSaveScripts = (void(__cdecl *)(int, int))CPatch::MakeCallAddr(0x61C763, 0x45F7D0);
		CPatch::RedirectCall(0x61C763, GtaGame::OnGameSaveScripts); //vc
		this->Events.pfDrawInMenu = (void(__cdecl *)(float, float, wchar_t *))CPatch::MakeCallAddr(0x49E3D9, 0x551040);
		CPatch::RedirectCall(0x49E3D9, GtaGame::OnMenuDrawing);
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
	game.Font.PrintString(ScreenCoord(60.0f), (float)*game.Screen.Height - ScreenCoord(34.0f), line);
	swprintf(line, L"%d %s, %d %s loaded", scriptMgr.numLoadedCustomScripts, scriptMgr.numLoadedCustomScripts == 1? L"script" : L"scripts",
		CleoPlugins::numLoadedPlugins, CleoPlugins::numLoadedPlugins == 1? L"plugin" : L"plugins");
	game.Font.PrintString(ScreenCoord(90.0f), (float)*game.Screen.Height - ScreenCoord(20.0f), line);
}