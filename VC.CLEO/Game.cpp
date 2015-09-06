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
#define GAME_ID_GTAVC_1_1 0x00408DC0
#define GAME_ID_GTAVC_STEAM 0x00004824
#define GAME_ID_GTAVC_STEAMENC 0x24E58287

DWORD WINAPI SteamHandler(LPVOID)
{
	while (true)
	{
		Sleep(0);
		if (GAME_VERSION_ID == GAME_ID_GTAVC_STEAM) break;
	}
	game.Version = GAME_VSTEAM;
	game.InitAndPatch();
	return 0;
}

GtaGame::GtaGame()
{
	this->InitialiseGameVersion();
	
	if (game.GetGameVersion() == GAME_VSTEAMENC)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SteamHandler, NULL, 0, NULL);
	}
	else
	{
		this->InitAndPatch();
	}
}

void GtaGame::InitialiseGameVersion()
{
	switch (GAME_VERSION_ID)
	{
	case GAME_ID_GTAVC_1_0:
		this->Version = GAME_V1_0;
		break;
	case GAME_ID_GTAVC_1_1:
		this->Version = GAME_V1_1;
		break;
	case GAME_ID_GTAVC_STEAM:
		this->Version = GAME_VSTEAM;
		break;
	case GAME_ID_GTAVC_STEAMENC:
		this->Version = GAME_VSTEAMENC;
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
	this->Misc.openedFiles = new std::set<FILE *>;
	this->Misc.allocatedMemory = new std::set<void *>;

	switch(this->Version)
	{
	case GAME_V1_0:
		// Scripts
		CPatch::SetPointer(0x4504E4, scriptMgr.gameScripts);
		CPatch::SetPointer(0x450508, scriptMgr.gameScripts);
		CPatch::SetPointer(0x45050E, (DWORD*)(scriptMgr.gameScripts)+1);
		CPatch::SetInt(0x450527 + 2, sizeof(CScript));
		CPatch::SetInt(0x45052D + 2, sizeof(CScript));
		CPatch::RedirectJump(0x450CF0, ScriptManager::InitialiseScript);
		CPatch::RedirectJump(0x44FBE0, ScriptManager::ProcessScriptCommand); 
		CPatch::RedirectJump(0x451010, ScriptManager::CollectScriptParameters);
		CPatch::RedirectJump(0x450EF0, ScriptManager::CollectScriptNextParameterWithoutIncreasingPC);
		this->Scripts.AddScriptToList = (void(__thiscall *)(CScript *, CScript **))0x4502E0;
		this->Scripts.RemoveScriptFromList = (void(__thiscall *)(CScript *, CScript **))0x450300;
		this->Scripts.StoreParameters = (void(__thiscall *)(CScript *, unsigned int *, unsigned int))0x450E50;
		this->Scripts.UpdateCompareFlag = (void(__thiscall *)(CScript *, bool))0x463F00;
		this->Scripts.GetPointerToScriptVariable = (void *(__thiscall *)(CScript *, unsigned int *, unsigned char))0x450DD0;
		this->Scripts.OpcodeHandlers[0] = (OpcodeHandler)0x44B400;
		this->Scripts.OpcodeHandlers[1] = (OpcodeHandler)0x446390;
		this->Scripts.OpcodeHandlers[2] = (OpcodeHandler)0x444BE0;
		this->Scripts.OpcodeHandlers[3] = (OpcodeHandler)0x453670;
		this->Scripts.OpcodeHandlers[4] = (OpcodeHandler)0x451F90;
		this->Scripts.OpcodeHandlers[5] = (OpcodeHandler)0x457580;
		this->Scripts.OpcodeHandlers[6] = (OpcodeHandler)0x456E20;
		this->Scripts.OpcodeHandlers[7] = (OpcodeHandler)0x455030;
		this->Scripts.OpcodeHandlers[8] = (OpcodeHandler)0x45B220;
		this->Scripts.OpcodeHandlers[9] = (OpcodeHandler)0x458EC0;
		this->Scripts.OpcodeHandlers[10] = (OpcodeHandler)0x6084C0;
		this->Scripts.OpcodeHandlers[11] = (OpcodeHandler)0x606730;
		this->Scripts.OpcodeHandlers[12] = (OpcodeHandler)0x630650;
		this->Scripts.OpcodeHandlers[13] = (OpcodeHandler)0x62E940;
		this->Scripts.OpcodeHandlers[14] = (OpcodeHandler)0x637600;
		this->Scripts.pActiveScriptsList = (CScript **)0x975338;
		this->Scripts.Params = (tScriptVar *)0x7D7438;
		this->Scripts.Space = (char *)0x821280;
		this->Scripts.pNumOpcodesExecuted = (unsigned short *)0xA10A66;
		// Text
		this->Text.pfGetText = (wchar_t *(__thiscall *)(int, char *))0x584DA2;
		CPatch::SetInt(0x584DA2, 0xD98B5553); //push ebx push ebp mov ebx,ecx
		CPatch::SetInt(0x584DA6, 0xE940EC83); //sub esp,40 
		CPatch::SetInt(0x584DAA, 0x00000189); //jmp 584F37
		CPatch::RedirectJump(0x584F30, CustomText::GetText);
		this->Text.CText = 0x94B220;
		this->Text.textDrawers = (CTextDrawer *)0x7F0EA0;
		this->Text.currentTextDrawer = (unsigned short *)0xA10A48;
		this->Text.cheatString = (char *)0xA10942;
		this->Text.TextBox = (void(__cdecl *)(const wchar_t *text, bool flag1, bool infinite, bool flag2))0x55BFC0;
		this->Text.StyledText = (void(__cdecl *)(const wchar_t *text, unsigned time, unsigned style))0x583F40;
		this->Text.TextLowPriority = (void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1, bool flag2))0x584410;
		this->Text.TextHighPriority = (void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1, bool flag2))0x584300;
		// Screen
		this->Screen.Width = (int *)0x9B48E4;
		this->Screen.Height = (int *)0x9B48E8;
		// Font
		this->Font.AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x552500;
		this->Font.PrintString = (void(__cdecl *)(float, float, wchar_t *)) 0x551040;
		this->Font.SetFontStyle = (void(__cdecl *)(int)) 0x54FFE0;
		this->Font.SetScale = (void(__cdecl *)(float, float)) 0x550230;
		this->Font.SetColor = (void(__cdecl *)(unsigned int *)) 0x550170;
		this->Font.SetLeftJustifyOn = (void(__cdecl *)()) 0x550040;
		this->Font.SetDropShadowPosition = (void(__cdecl *)(int)) 0x54FF20;
		this->Font.SetPropOn = (void(__cdecl *)()) 0x550020;
		// Pools
		this->Pools.pPedPool = (GamePool **)0x97F2AC;
		this->Pools.pVehiclePool = (GamePool **)0xA0FDE4;
		this->Pools.pObjectPool = (GamePool **)0x94DBE0;
		this->Pools.pfPedPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451CB0;
		this->Pools.pfVehiclePoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451C70;
		this->Pools.pfObjectPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451C30;
		this->Pools.pfPedPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x451CF0;
		this->Pools.pfVehiclePoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x42C4B0;
		this->Pools.pfObjectPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x434A10;
		// Events
		this->Events.pfInitScripts_OnGameSaveLoad = (void(__cdecl *)())CPatch::MakeCallAddr(0x45F463, 0x450330);
		CPatch::RedirectCall(0x45F463, GtaGame::InitScripts_OnGameSaveLoad);
		this->Events.pfInitScripts_OnGameInit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A492F, 0x450330);
		CPatch::RedirectCall(0x4A492F, GtaGame::InitScripts_OnGameInit);
		this->Events.pfInitScripts_OnGameReinit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4E96, 0x450330);
		CPatch::RedirectCall(0x4A4E96, GtaGame::InitScripts_OnGameReinit);
		this->Events.pfShutdownGame = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4AFF, 0x408150);
		CPatch::RedirectCall(0x4A4AFF, GtaGame::OnShutdownGame);
		this->Events.pfGameSaveScripts = (void(__cdecl *)(int, int))CPatch::MakeCallAddr(0x61C763, 0x45F7D0);
		CPatch::RedirectCall(0x61C763, GtaGame::OnGameSaveScripts);
		this->Events.pfDrawInMenu = (void(__cdecl *)(float, float, wchar_t *))CPatch::MakeCallAddr(0x49E3D9, 0x551040);
		CPatch::RedirectCall(0x49E3D9, GtaGame::OnMenuDrawing);
		//Shadows
		this->Shadows.StoreShadowToBeRendered = (float(__cdecl *)(unsigned char, uintptr_t *, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, bool, float, uintptr_t *, bool)) 0x56E6C0;
		this->Shadows.pRwTexture_shad_car     = (uintptr_t **)0x97F2EC;
		this->Shadows.pRwTexture_shad_ped	  = (uintptr_t **)0x9B5F2C;
		this->Shadows.pRwTexture_shad_heli	  = (uintptr_t **)0x975218;
		this->Shadows.pRwTexture_shad_bike	  = (uintptr_t **)0x94DBC0;
		this->Shadows.pRwTexture_shad_rcbaron = (uintptr_t **)0x94DBD4;
		this->Shadows.pRwTexture_shad_exp	  = (uintptr_t **)0x978DB4;
		this->Shadows.pRwTexture_headlight	  = (uintptr_t **)0xA1073C;
		this->Shadows.pRwTexture_bloodpool_64 = (uintptr_t **)0xA0DAC8;
		//Misc
		this->Misc.stVehicleModelInfo = 0x752A8C;
		this->Misc.activePadState = 0x7DBCB0;
		this->Misc.pfModelForWeapon = (int(__cdecl *)(int eWeaponType)) 0x4418B0;
		this->Misc.cameraWidescreen = 0x7E46F5;
		this->Misc.currentWeather = 0xA10AAA;
		this->Misc.pfGetUserDirectory = (char*(__cdecl *)()) 0x602240;
		this->Misc.pfSpawnCar = (void(__cdecl *)(unsigned int modelID)) 0x4AE8F0;
		this->Misc.pfCAnimManagerBlendAnimation = (int(__cdecl *)(int pRpClump, int dwAnimGroupId, int dwAnimId, float fSpeed)) 0x405640;
		break;
	case GAME_V1_1:
		// Scripts
		CPatch::SetPointer(0x4504E4, scriptMgr.gameScripts);
		CPatch::SetPointer(0x450508, scriptMgr.gameScripts);
		CPatch::SetPointer(0x45050E, (DWORD*)(scriptMgr.gameScripts) + 1);
		CPatch::SetInt(0x450527 + 2, sizeof(CScript));
		CPatch::SetInt(0x45052D + 2, sizeof(CScript));
		CPatch::RedirectJump(0x450CF0, ScriptManager::InitialiseScript);
		CPatch::RedirectJump(0x44FBE0, ScriptManager::ProcessScriptCommand);
		CPatch::RedirectJump(0x451010, ScriptManager::CollectScriptParameters);
		CPatch::RedirectJump(0x450EF0, ScriptManager::CollectScriptNextParameterWithoutIncreasingPC);
		this->Scripts.AddScriptToList = (void(__thiscall *)(CScript *, CScript **))0x4502E0;
		this->Scripts.RemoveScriptFromList = (void(__thiscall *)(CScript *, CScript **))0x450300;
		this->Scripts.StoreParameters = (void(__thiscall *)(CScript *, unsigned int *, unsigned int))0x450E50;
		this->Scripts.UpdateCompareFlag = (void(__thiscall *)(CScript *, bool))0x463F00;
		this->Scripts.GetPointerToScriptVariable = (void *(__thiscall *)(CScript *, unsigned int *, unsigned char))0x450DD0;
		this->Scripts.OpcodeHandlers[0] = (OpcodeHandler)0x44B400;
		this->Scripts.OpcodeHandlers[1] = (OpcodeHandler)0x446390;
		this->Scripts.OpcodeHandlers[2] = (OpcodeHandler)0x444BE0;
		this->Scripts.OpcodeHandlers[3] = (OpcodeHandler)0x453670;
		this->Scripts.OpcodeHandlers[4] = (OpcodeHandler)0x451F90;
		this->Scripts.OpcodeHandlers[5] = (OpcodeHandler)0x457580;
		this->Scripts.OpcodeHandlers[6] = (OpcodeHandler)0x456E20;
		this->Scripts.OpcodeHandlers[7] = (OpcodeHandler)0x455030;
		this->Scripts.OpcodeHandlers[8] = (OpcodeHandler)0x45B220;
		this->Scripts.OpcodeHandlers[9] = (OpcodeHandler)0x458EC0;
		this->Scripts.OpcodeHandlers[10] = (OpcodeHandler)0x6084A0;
		this->Scripts.OpcodeHandlers[11] = (OpcodeHandler)0x606710;
		this->Scripts.OpcodeHandlers[12] = (OpcodeHandler)0x6306A0;
		this->Scripts.OpcodeHandlers[13] = (OpcodeHandler)0x62E990;
		this->Scripts.OpcodeHandlers[14] = (OpcodeHandler)0x637650;
		this->Scripts.pActiveScriptsList = (CScript **)0x975340;
		this->Scripts.Params = (tScriptVar *)(0x7D7438 + 0x8);
		this->Scripts.Space = (char *)(0x821280 + 0x8);
		this->Scripts.pNumOpcodesExecuted = (unsigned short *)0xA10A6E;
		// Text
		this->Text.pfGetText = (wchar_t *(__thiscall *)(int, char *))0x584DC2;
		CPatch::SetInt(0x584DC2, 0xD98B5553); //push ebx push ebp mov ebx,ecx
		CPatch::SetInt(0x584DC6, 0xE940EC83); //sub esp,40 
		CPatch::SetInt(0x584DCA, 0x00000189); //jmp 584F37
		CPatch::RedirectJump(0x584F50, CustomText::GetText);
		this->Text.CText = 0x94B228;
		this->Text.textDrawers = (CTextDrawer *)0x7F0EA8;
		this->Text.currentTextDrawer = (unsigned short *)0xA10A50;
		this->Text.cheatString = (char *)(0xA10942 + 0x8);
		this->Text.TextBox = (void(__cdecl *)(const wchar_t *text, bool flag1, bool infinite, bool flag2))0x55BFE0;
		this->Text.StyledText = (void(__cdecl *)(const wchar_t *text, unsigned time, unsigned style))0x583F60;
		this->Text.TextLowPriority = (void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1, bool flag2))0x584430;
		this->Text.TextHighPriority = (void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1, bool flag2))0x584320;
		// Screen
		this->Screen.Width = (int *)0x9B48EC;
		this->Screen.Height = (int *)0x9B48F0;
		// Font
		this->Font.AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x552520;
		this->Font.PrintString = (void(__cdecl *)(float, float, wchar_t *)) 0x551060;
		this->Font.SetFontStyle = (void(__cdecl *)(int)) 0x550000;
		this->Font.SetScale = (void(__cdecl *)(float, float)) 0x550250;
		this->Font.SetColor = (void(__cdecl *)(unsigned int *)) 0x550190;
		this->Font.SetLeftJustifyOn = (void(__cdecl *)()) 0x550060;
		this->Font.SetDropShadowPosition = (void(__cdecl *)(int)) 0x54FF40;
		this->Font.SetPropOn = (void(__cdecl *)()) 0x550040;
		// Pools
		this->Pools.pPedPool = (GamePool **)(0x97F2AC + 0x8);
		this->Pools.pVehiclePool = (GamePool **)(0xA0FDE4 + 0x8);
		this->Pools.pObjectPool = (GamePool **)(0x94DBE0 + 0x8);
		this->Pools.pfPedPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451CB0;
		this->Pools.pfVehiclePoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451C70;
		this->Pools.pfObjectPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451C30;
		this->Pools.pfPedPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x451CF0;
		this->Pools.pfVehiclePoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x42C4B0;
		this->Pools.pfObjectPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x434A10;
		// Events
		this->Events.pfInitScripts_OnGameSaveLoad = (void(__cdecl *)())CPatch::MakeCallAddr(0x45F463, 0x450330);
		CPatch::RedirectCall(0x45F463, GtaGame::InitScripts_OnGameSaveLoad);
		this->Events.pfInitScripts_OnGameInit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A494F, 0x450330);
		CPatch::RedirectCall(0x4A494F, GtaGame::InitScripts_OnGameInit);
		this->Events.pfInitScripts_OnGameReinit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4EB6, 0x450330);
		CPatch::RedirectCall(0x4A4EB6, GtaGame::InitScripts_OnGameReinit);
		this->Events.pfShutdownGame = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4AFF + 0x20, 0x408150);
		CPatch::RedirectCall(0x4A4AFF + 0x20, GtaGame::OnShutdownGame);
		this->Events.pfGameSaveScripts = (void(__cdecl *)(int, int))CPatch::MakeCallAddr(0x61C743, 0x45F7D0);
		CPatch::RedirectCall(0x61C743, GtaGame::OnGameSaveScripts);
		this->Events.pfDrawInMenu = (void(__cdecl *)(float, float, wchar_t *))CPatch::MakeCallAddr(0x49E3FA, 0x551060);
		CPatch::RedirectCall(0x49E3FA, GtaGame::OnMenuDrawing);
		//Shadows
		this->Shadows.StoreShadowToBeRendered = (float(__cdecl *)(unsigned char, uintptr_t *, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, bool, float, uintptr_t *, bool)) (0x56E6C0 + 0x20);
		this->Shadows.pRwTexture_shad_car = (uintptr_t **)(0x97F2EC + 0x8);
		this->Shadows.pRwTexture_shad_ped = (uintptr_t **)(0x9B5F2C + 0x8);
		this->Shadows.pRwTexture_shad_heli = (uintptr_t **)(0x975218 + 0x8);
		this->Shadows.pRwTexture_shad_bike = (uintptr_t **)(0x94DBC0 + 0x8);
		this->Shadows.pRwTexture_shad_rcbaron = (uintptr_t **)(0x94DBD4 + 0x8);
		this->Shadows.pRwTexture_shad_exp = (uintptr_t **)(0x978DB4 + 0x8);
		this->Shadows.pRwTexture_headlight = (uintptr_t **)(0xA1073C + 0x8);
		this->Shadows.pRwTexture_bloodpool_64 = (uintptr_t **)(0xA0DAC8 + 0x8);
		//Misc
		this->Misc.stVehicleModelInfo = 0x752A8C;
		this->Misc.activePadState = 0x7DBCB0 + 0x8;
		this->Misc.pfModelForWeapon = (int(__cdecl *)(int eWeaponType)) 0x4418B0;
		this->Misc.cameraWidescreen = 0x7E46F5 + 0x8;
		this->Misc.currentWeather = 0xA10AAA + 0x8;
		this->Misc.pfGetUserDirectory = (char*(__cdecl *)()) 0x602220;
		this->Misc.pfSpawnCar = (void(__cdecl *)(unsigned int modelID)) 0x4AE7D0;
		this->Misc.pfCAnimManagerBlendAnimation = (int(__cdecl *)(int pRpClump, int dwAnimGroupId, int dwAnimId, float fSpeed)) 0x405640;
		break;
	case GAME_VSTEAM:
		// Scripts
		CPatch::SetPointer(0x4503F4, scriptMgr.gameScripts);
		CPatch::SetPointer(0x450418, scriptMgr.gameScripts);
		CPatch::SetPointer(0x45041E, (DWORD*)(scriptMgr.gameScripts) + 1);
		CPatch::SetInt(0x450437 + 2, sizeof(CScript));
		CPatch::SetInt(0x45043D + 2, sizeof(CScript));
		CPatch::RedirectJump(0x450C00, ScriptManager::InitialiseScript);
		CPatch::RedirectJump(0x44FAF0, ScriptManager::ProcessScriptCommand);
		CPatch::RedirectJump(0x450F20, ScriptManager::CollectScriptParameters);
		CPatch::RedirectJump(0x450E00, ScriptManager::CollectScriptNextParameterWithoutIncreasingPC);
		this->Scripts.AddScriptToList = (void(__thiscall *)(CScript *, CScript **))0x4501F0;
		this->Scripts.RemoveScriptFromList = (void(__thiscall *)(CScript *, CScript **))0x450210;
		this->Scripts.StoreParameters = (void(__thiscall *)(CScript *, unsigned int *, unsigned int))0x450D60;
		this->Scripts.UpdateCompareFlag = (void(__thiscall *)(CScript *, bool))0x463DE0;
		this->Scripts.GetPointerToScriptVariable = (void *(__thiscall *)(CScript *, unsigned int *, unsigned char))0x450CE0;
		this->Scripts.OpcodeHandlers[0] = (OpcodeHandler)0x44B310;
		this->Scripts.OpcodeHandlers[1] = (OpcodeHandler)0x4462A0;
		this->Scripts.OpcodeHandlers[2] = (OpcodeHandler)0x444AF0;
		this->Scripts.OpcodeHandlers[3] = (OpcodeHandler)0x453550;
		this->Scripts.OpcodeHandlers[4] = (OpcodeHandler)0x451E70;
		this->Scripts.OpcodeHandlers[5] = (OpcodeHandler)0x457460;
		this->Scripts.OpcodeHandlers[6] = (OpcodeHandler)0x456D00;
		this->Scripts.OpcodeHandlers[7] = (OpcodeHandler)0x454F10;
		this->Scripts.OpcodeHandlers[8] = (OpcodeHandler)0x45B100;
		this->Scripts.OpcodeHandlers[9] = (OpcodeHandler)0x458DA0;
		this->Scripts.OpcodeHandlers[10] = (OpcodeHandler)0x6080E0;
		this->Scripts.OpcodeHandlers[11] = (OpcodeHandler)0x606350;
		this->Scripts.OpcodeHandlers[12] = (OpcodeHandler)0x630310;
		this->Scripts.OpcodeHandlers[13] = (OpcodeHandler)0x62E600;
		this->Scripts.OpcodeHandlers[14] = (OpcodeHandler)0x6372C0;
		this->Scripts.pActiveScriptsList = (CScript **)0x974340;
		this->Scripts.Params = (tScriptVar *)0x7D6440;
		this->Scripts.Space = (char *)0x820288;
		this->Scripts.pNumOpcodesExecuted = (unsigned short *)0xA0FA6E;
		// Text
		this->Text.pfGetText = (wchar_t *(__thiscall *)(int, char *))0x584BD2;
		CPatch::SetInt(0x584BD2, 0xD98B5553); //push ebx push ebp mov ebx,ecx
		CPatch::SetInt(0x584BD6, 0xE940EC83); //sub esp,40 
		CPatch::SetInt(0x584BDA, 0x00000189); //jmp 584F37
		CPatch::RedirectJump(0x584D60, CustomText::GetText);
		this->Text.CText = 0x94A228;
		this->Text.textDrawers = (CTextDrawer *)0x7EFEA8;
		this->Text.currentTextDrawer = (unsigned short *)0xA0FA50;
		this->Text.cheatString = (char *)0xA0F94A;
		this->Text.TextBox = (void(__cdecl *)(const wchar_t *text, bool flag1, bool infinite, bool flag2))0x55BEB0;
		this->Text.StyledText = (void(__cdecl *)(const wchar_t *text, unsigned time, unsigned style))0x583D70;
		this->Text.TextLowPriority = (void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1, bool flag2))0x584240;
		this->Text.TextHighPriority = (void(__cdecl *)(const wchar_t *text, unsigned time, bool flag1, bool flag2))0x584130;
		// Screen
		this->Screen.Width = (int *)0x9B38EC;
		this->Screen.Height = (int *)0x9B38F0;
		// Font
		this->Font.AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x5523F0;
		this->Font.PrintString = (void(__cdecl *)(float, float, wchar_t *)) 0x550F30;
		this->Font.SetFontStyle = (void(__cdecl *)(int)) 0x54FED0;
		this->Font.SetScale = (void(__cdecl *)(float, float)) 0x550120;
		this->Font.SetColor = (void(__cdecl *)(unsigned int *)) 0x550060;
		this->Font.SetLeftJustifyOn = (void(__cdecl *)()) 0x54FF30;
		this->Font.SetDropShadowPosition = (void(__cdecl *)(int)) 0x54FE10;
		this->Font.SetPropOn = (void(__cdecl *)()) 0x54FF10;
		// Pools
		this->Pools.pPedPool = (GamePool **)0x97E2B4;
		this->Pools.pVehiclePool = (GamePool **)0xA0EDEC;
		this->Pools.pObjectPool = (GamePool **)0x94CBE8;
		this->Pools.pfPedPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451B90;
		this->Pools.pfVehiclePoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451B50;
		this->Pools.pfObjectPoolGetStruct = (void* (__thiscall *)(GamePool *, int))0x451B10;
		this->Pools.pfPedPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x451BD0;
		this->Pools.pfVehiclePoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x42C480;
		this->Pools.pfObjectPoolGetHandle = (int(__thiscall *)(GamePool *, void *))0x4349D0;
		// Events
		this->Events.pfInitScripts_OnGameSaveLoad = (void(__cdecl *)())CPatch::MakeCallAddr(0x45F343, 0x450240);
		CPatch::RedirectCall(0x45F343, GtaGame::InitScripts_OnGameSaveLoad);
		this->Events.pfInitScripts_OnGameInit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A47EF, 0x450240);
		CPatch::RedirectCall(0x4A47EF, GtaGame::InitScripts_OnGameInit);
		this->Events.pfInitScripts_OnGameReinit = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A4D63, 0x450240);
		CPatch::RedirectCall(0x4A4D63, GtaGame::InitScripts_OnGameReinit);
		this->Events.pfShutdownGame = (void(__cdecl *)())CPatch::MakeCallAddr(0x4A49BF, 0x408150);
		CPatch::RedirectCall(0x4A49BF, GtaGame::OnShutdownGame);
		this->Events.pfGameSaveScripts = (void(__cdecl *)(int, int))CPatch::MakeCallAddr(0x61C3A3, 0x45F6B0);
		CPatch::RedirectCall(0x61C3A3, GtaGame::OnGameSaveScripts);
		this->Events.pfDrawInMenu = (void(__cdecl *)(float, float, wchar_t *))CPatch::MakeCallAddr(0x49E298, 0x550F30);
		CPatch::RedirectCall(0x49E298, GtaGame::OnMenuDrawing);
		//Shadows
		this->Shadows.StoreShadowToBeRendered = (float(__cdecl *)(unsigned char, uintptr_t *, CVector *, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, bool, float, uintptr_t *, bool)) 0x56E5B0;
		this->Shadows.pRwTexture_shad_car = (uintptr_t **)0x97E2F4;
		this->Shadows.pRwTexture_shad_ped = (uintptr_t **)0x9B4F34;
		this->Shadows.pRwTexture_shad_heli = (uintptr_t **)0x974220;
		this->Shadows.pRwTexture_shad_bike = (uintptr_t **)0x94CBC8;
		this->Shadows.pRwTexture_shad_rcbaron = (uintptr_t **)0x94CBDC;
		this->Shadows.pRwTexture_shad_exp = (uintptr_t **)0x977DBC;
		this->Shadows.pRwTexture_headlight = (uintptr_t **)0xA0F744;
		this->Shadows.pRwTexture_bloodpool_64 = (uintptr_t **)0xA0CAD0;
		//Misc
		this->Misc.stVehicleModelInfo = 0x751A8C;
		this->Misc.activePadState = 0x7DACB8;
		this->Misc.pfModelForWeapon = (int(__cdecl *)(int eWeaponType)) 0x441820;
		this->Misc.cameraWidescreen = 0x7E36FD;
		this->Misc.currentWeather = 0xA0FAB2;
		this->Misc.pfGetUserDirectory = (char*(__cdecl *)()) 0x601E60;
		this->Misc.pfSpawnCar = (void(__cdecl *)(unsigned int modelID)) 0x4AE7C0;
		this->Misc.pfCAnimManagerBlendAnimation = (int(__cdecl *)(int pRpClump, int dwAnimGroupId, int dwAnimId, float fSpeed)) 0x405640;
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
	std::for_each(game.Misc.openedFiles->begin(), game.Misc.openedFiles->end(), fclose);
	game.Misc.openedFiles->clear();
	std::for_each(game.Misc.allocatedMemory->begin(), game.Misc.allocatedMemory->end(), free);
	game.Misc.allocatedMemory->clear();
}

void GtaGame::InitScripts_OnGameSaveLoad()
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Load Save--");
	scriptMgr.UnloadScripts();
	CustomText::Unload();
	game.Events.pfInitScripts_OnGameSaveLoad();
	scriptMgr.LoadScripts();
	CustomText::Load();
	std::for_each(game.Misc.openedFiles->begin(), game.Misc.openedFiles->end(), fclose);
	game.Misc.openedFiles->clear();
	std::for_each(game.Misc.allocatedMemory->begin(), game.Misc.allocatedMemory->end(), free);
	game.Misc.allocatedMemory->clear();
}

void GtaGame::OnShutdownGame()
{
	LOGL(LOG_PRIORITY_GAME_EVENT, "--Game Shutdown--");
	game.Events.pfShutdownGame();
	scriptMgr.UnloadScripts();
	CustomText::Unload();
	std::for_each(game.Misc.openedFiles->begin(), game.Misc.openedFiles->end(), fclose);
	game.Misc.openedFiles->clear();
	std::for_each(game.Misc.allocatedMemory->begin(), game.Misc.allocatedMemory->end(), free);
	game.Misc.allocatedMemory->clear();
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
	unsigned char color[4] = { 0xFF, 0x96, 0xE1, 0xFF };
	game.Font.SetColor((unsigned int *)color);
	game.Font.SetDropShadowPosition(0);
	game.Font.SetPropOn();
	game.Font.SetFontStyle(0);
	game.Font.SetScale(ScreenCoord(0.45f), ScreenCoord(0.7f));
	game.Font.SetLeftJustifyOn();
	wchar_t line[128];
	swprintf(line, L"CLEO v%d.%d.%d.%d", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);
	game.Font.PrintString(ScreenCoord(30.0f), (float)*game.Screen.Height - ScreenCoord(34.0f), line);
	scriptMgr.numLoadedCustomScripts ?
	swprintf(line, L"%d %s, %d %s loaded", scriptMgr.numLoadedCustomScripts, scriptMgr.numLoadedCustomScripts == 1? L"script" : L"scripts",
		CleoPlugins::numLoadedPlugins, CleoPlugins::numLoadedPlugins == 1? L"plugin" : L"plugins") :
	swprintf(line, L"%d %s loaded", CleoPlugins::numLoadedPlugins, CleoPlugins::numLoadedPlugins == 1 ? L"plugin" : L"plugins");
	game.Font.PrintString(ScreenCoord(30.0f), (float)*game.Screen.Height - ScreenCoord(20.0f), line);
}