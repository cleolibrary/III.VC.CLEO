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
		LOGL(LOG_PRIORITY_ALWAYS, "GTA VC CLEO v%d.%d.%d.%d Log File", CLEO_VERSION_MAIN, CLEO_VERSION_MAJOR, 
			CLEO_VERSION_MINOR, CLEO_VERSION_BINARY);
		CustomOpcodes::Register();
		CleoPlugins::LoadPlugins();
	}
	else if(fdwReason == DLL_PROCESS_DETACH)
	{
		CleoPlugins::UnloadPlugins();
		Log::Close();
	}
	return TRUE;
}