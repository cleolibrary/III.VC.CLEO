#include <Windows.h>
#include "Game.h"
#include "CPatch.h"
#include "Log.h"
#include "CustomOpcodes.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"

DWORD WINAPI SteamHandlerDllMain(LPVOID)
{
	while (true)
	{
		Sleep(0);
		if (game.GetGameVersion() == GAME_VSTEAM) break;
	}
	CustomOpcodes::Register();
	CleoPlugins::LoadPlugins();
	return 0;
}

BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		Log::Initialise("cleo.log");
		LOGL(LOG_PRIORITY_ALWAYS, "GTA VC CLEO v%d.%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, 
			CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);
		switch (game.GetGameVersion())
		{
		case GAME_V1_0:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC v%d.%d", 1, 0);
			break;
		case GAME_V1_1:
			LOGL(LOG_PRIORITY_GAME_EVENT, "GTA VC v%d.%d", 1, 1);
			break;
		case GAME_VSTEAM:
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