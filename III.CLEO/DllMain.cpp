#include <Windows.h>
#include "Game.h"
#include "CPatch.h"
#include "Log.h"
#include "CustomOpcodes.h"
#include "CleoVersion.h"
#include "CleoPlugins.h"

BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{
		Log::Initialise("cleo.log");
		LOGL(LOG_PRIORITY_ALWAYS, "GTA 3 CLEO v%d.%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, 
			CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);
		CustomOpcodes::Register();
		CleoPlugins::LoadPlugins();
		DWORD attr = GetFileAttributes("audio\\HEAD.wav");
		if(attr != -1 && !(attr & FILE_ATTRIBUTE_DIRECTORY))
		{
			switch(game.Version)
			{
			case GAME_V1_1:
				CPatch::SetChar(0x566B55, 0);
				CPatch::Nop(0x566B96, 6);
				CPatch::Nop(0x581F84, 2);
				CPatch::Nop(0x581F92, 6);
				*(unsigned char *)0x6043EC = 0;
				break;
			default:
				CPatch::SetChar(0x566A15, 0);
				CPatch::Nop(0x566A56, 6);
				CPatch::Nop(0x581C44, 2);
				CPatch::Nop(0x581C52, 6);
				*(unsigned char *)0x604B24 = 0;
			}
		}
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		CleoPlugins::UnloadPlugins();
		Log::Close();
	}
	return TRUE;
}