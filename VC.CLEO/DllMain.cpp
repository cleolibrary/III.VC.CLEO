#include <Windows.h>
#include "Game.h"
#include "CPatch.h"
#include "Log.h"
#include "CustomOpcodes.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"
#include <vector>
#include "..\injector\hooking.hpp"

template<uintptr_t addr>
void RwRenderStateSetHook()
{
	using func_hook = injector::function_hooker<addr, void(int, int)>;
	injector::make_static_hook<func_hook>([](func_hook::func_type RwRenderStateSet, int a1, int a2)
	{
		RwRenderStateSet(a1, a2);
		RwRenderStateSet(0xC, a2);
		return;
	});
}

DWORD WINAPI SteamHandlerDllMain(LPVOID)
{
	while (true)
	{
		Sleep(0);
		if (game.GetGameVersion() == GAME_VSTEAM) break;
	}
	RwRenderStateSetHook<0x5786A5>();
	CustomOpcodes::Register();
	CleoPlugins::LoadPlugins();
	return 0;
}

std::vector<char>	aScriptTextures;
std::vector<char>	aTextSprites;

void PatchArrays()
{
	aScriptTextures.resize(4 * 121);
	aTextSprites.resize(0x18 * 121);

	injector::AdjustPointer(0x450B0E, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> mov     edi, offset _scriptTextures
	injector::AdjustPointer(0x450C85, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> mov     esi, offset _scriptTextures
	injector::AdjustPointer(0x451668, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> mov     esi, offset _scriptTextures
	//injector::AdjustPointer(0x451EA1, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> push    offset _scriptTextures; object ctor
	//injector::AdjustPointer(0x451EDA, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> push    offset _scriptTextures; objects dtor
	injector::AdjustPointer(0x4593C7, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> add     ecx, offset _scriptTextures; this
	injector::AdjustPointer(0x5569AD, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> add     ecx, offset _scriptTextures; this
	injector::AdjustPointer(0x55ADFC, &aScriptTextures[0], 0x8117E8, 0x8117E8 + 0x0); //  -> add     ecx, offset _scriptTextures; this

	injector::WriteMemory<char>(0x450B1D + 3, 121, true);
	injector::WriteMemory<char>(0x450C9B + 3, 121, true);
	//injector::WriteMemory<char>(0x45167E + 3, 121, true); //CMissionCleanup::Process()
	injector::WriteMemory<unsigned char>(0x451692, 0xEB, true); //CMissionCleanup::Process()
	injector::WriteMemory<char>(0x5569BD + 3, 121, true);
	injector::WriteMemory<char>(0x55AE0C + 3, 121, true);


	injector::AdjustPointer(0x450125, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[eax], 0
	injector::AdjustPointer(0x450146, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x450164, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ecx], 0
	injector::AdjustPointer(0x450183, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 0
	injector::AdjustPointer(0x4501A1, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x4501BF, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ecx], 0
	injector::AdjustPointer(0x4501DE, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 0
	injector::AdjustPointer(0x450203, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x450A78, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ds:_textSprites.active[ebx], 0
	//injector::AdjustPointer(0x451E72, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> push    offset _textSprites; object
	//injector::AdjustPointer(0x451EFA, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> push    offset _textSprites; objects
	injector::AdjustPointer(0x45918E, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 1
	injector::AdjustPointer(0x4591FB, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> add     esi, offset _textSprites
	injector::AdjustPointer(0x45929E, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     byte ptr ss:_textSprites.active[ebp], 1
	injector::AdjustPointer(0x459306, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> add     esi, offset _textSprites
	injector::AdjustPointer(0x55690B, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     esi, offset _textSprites
	injector::AdjustPointer(0x556912, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> cmp     byte ptr ss:_textSprites.active[ebp], 0
	injector::AdjustPointer(0x55AD3C, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> mov     ebp, offset _textSprites
	injector::AdjustPointer(0x55AD42, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x0); //  -> cmp     byte ptr ds:_textSprites.active[ebx], 0
	injector::AdjustPointer(0x45012D, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[eax], 0
	injector::AdjustPointer(0x45014D, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x45016B, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ecx], 0
	injector::AdjustPointer(0x45018A, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ss:_textSprites.antialiased[ebp], 0
	injector::AdjustPointer(0x4501A8, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x4501C6, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ecx], 0
	injector::AdjustPointer(0x4501E5, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ss:_textSprites.antialiased[ebp], 0
	injector::AdjustPointer(0x45020A, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x450A93, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x45B07C, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[eax*8], 1
	injector::AdjustPointer(0x45B090, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> mov     ds:_textSprites.antialiased[eax*8], 0
	injector::AdjustPointer(0x55691F, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> cmp     ss:_textSprites.antialiased[ebp], 0
	injector::AdjustPointer(0x55AD4F, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x1); //  -> cmp     ds:_textSprites.antialiased[ebx], 0
	injector::AdjustPointer(0x450A9B, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x2); //  -> or      ds:_textSprites.textureID[ebx], 0FFFFh
	injector::AdjustPointer(0x459196, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x2); //  -> mov     ss:_textSprites.textureID[ebp], ax
	injector::AdjustPointer(0x4592A6, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x2); //  -> or      ss:_textSprites.textureID[ebp], 0FFFFh
	injector::AdjustPointer(0x55692D, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x2); //  -> mov     dx, ss:_textSprites.textureID[ebp]
	injector::AdjustPointer(0x55AD5D, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x2); //  -> mov     dx, ds:_textSprites.textureID[ebx]
	injector::AdjustPointer(0x450AA2, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> mov     dword ptr ds:_textSprites.pos.x[ebx], 0
	injector::AdjustPointer(0x45919C, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> fstp    dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x4592AD, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> fstp    dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x556938, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x556972, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ss:_textSprites.pos.x[ebp]
	injector::AdjustPointer(0x55AD68, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ds:_textSprites.pos.x[ebx]
	injector::AdjustPointer(0x55ADB2, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x4); //  -> fld     dword ptr ds:_textSprites.pos.x[ebx]
	injector::AdjustPointer(0x450AAC, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> mov     ds:_textSprites.pos.y[ebx], 0
	injector::AdjustPointer(0x4591A6, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> fstp    ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x4592B7, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> fstp    ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x556942, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x55697C, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ss:_textSprites.pos.y[ebp]
	injector::AdjustPointer(0x55AD75, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ds:_textSprites.pos.y[ebx]
	injector::AdjustPointer(0x55ADBF, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x8); //  -> fld     ds:_textSprites.pos.y[ebx]
	injector::AdjustPointer(0x450AB6, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> mov     ds:_textSprites.pos.w[ebx], 0
	injector::AdjustPointer(0x4591BB, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> fstp    ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x4592C1, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> fstp    ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x55694C, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x556986, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ss:_textSprites.pos.w[ebp]
	injector::AdjustPointer(0x55AD82, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ds:_textSprites.pos.w[ebx]
	injector::AdjustPointer(0x55ADCC, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0xC); //  -> fld     ds:_textSprites.pos.w[ebx]
	injector::AdjustPointer(0x450AC0, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> mov     ds:_textSprites.pos.h[ebx], 0
	injector::AdjustPointer(0x4591CB, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> fstp    ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x4592D6, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> fstp    ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x556956, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x556990, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ss:_textSprites.pos.h[ebp]
	injector::AdjustPointer(0x55AD8F, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ds:_textSprites.pos.h[ebx]
	injector::AdjustPointer(0x55ADD9, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x10); //  -> fld     ds:_textSprites.pos.h[ebx]
	injector::AdjustPointer(0x450AD8, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x14); //  -> mov     byte ptr ds:_textSprites.transparentColor.red[ebx], cl
	injector::AdjustPointer(0x450AE2, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x15); //  -> mov     ds:_textSprites.transparentColor.green[ebx], al
	injector::AdjustPointer(0x450AEC, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x16); //  -> mov     ds:_textSprites.transparentColor.blue[ebx], dl
	injector::AdjustPointer(0x450AF2, &aTextSprites[0], 0x813DF0, 0x813DF0 + 0x17); //  -> mov     ds:_textSprites.transparentColor.alpha[ebx], al

	injector::WriteMemory<char>(0x4501F3 + 3, 121, true);
	injector::WriteMemory<char>(0x450AF9 + 2, 121, true);
	injector::WriteMemory<char>(0x4501F3 + 3, 121, true);
}

BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		Log::Initialise("cleo.log");
		LOGL(LOG_PRIORITY_ALWAYS, "GTA VC CLEO v%d.%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, 
			CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);

		switch (game.Version)
		{
		case GAME_V1_0:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC v%d.%d", 1, 0);
			RwRenderStateSetHook<0x578737>();
			RwRenderStateSetHook<0x578737 - 0x50>();
			PatchArrays();
			break;
		case GAME_V1_1:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC v%d.%d", 1, 1);
			RwRenderStateSetHook<0x578757>();
			RwRenderStateSetHook<0x578757 - 0x50>();
			break;
		case GAME_VSTEAM:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC steam version");
			RwRenderStateSetHook<0x5786A5>();
			RwRenderStateSetHook<0x5786A5 - 0x7E>();
			break;
		case GAME_VSTEAMENC:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC steam version");
			break;
		default:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC unknown version");
			break;
		}

		if (game.GetGameVersion() == GAME_VSTEAMENC)
		{
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&SteamHandlerDllMain, NULL, 0, NULL);
		}
		else
		{
			CustomOpcodes::Register();
			CleoPlugins::LoadPlugins();
		}
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		CleoPlugins::UnloadPlugins();
		Log::Close();
	}
	return TRUE;
}