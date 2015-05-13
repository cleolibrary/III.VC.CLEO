#include "CleoPlugins.h"
#include "Log.h"

CleoPlugin *CleoPlugins::plugins = 0;
unsigned int CleoPlugins::numLoadedPlugins = 0;

CleoPlugin::CleoPlugin(char *pluginname, char *librarypath)
{
	this->name = new char[strlen(pluginname) + 1];
	strcpy(this->name, pluginname);
	this->module = LoadLibrary(librarypath);
}

CleoPlugin::~CleoPlugin()
{
	delete[] name;
	FreeLibrary(this->module);
}

void CleoPlugins::LoadPlugins()
{
	WIN32_FIND_DATA FindFileData;
	memset(&FindFileData, 0, sizeof(WIN32_FIND_DATA));
	HANDLE hFind = FindFirstFile("CLEO\\*.cleo", &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char filename[MAX_PATH];
				strcpy(filename, "CLEO\\");
				strcat(filename, FindFileData.cFileName);
				CleoPlugin *plugin = new CleoPlugin(FindFileData.cFileName, filename);
				if(plugin)
				{
					if(!plugin->module)
					{
						delete plugin;
						LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Failed to load plugin %s", plugin->name);
					}
					else
					{
						plugin->next = plugins;
						plugins = plugin;
						numLoadedPlugins++;
						LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Loaded plugin %s", plugin->name);
					}
				}
				else
					LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Failed to load plugin %s", FindFileData.cFileName);
			}
		}
		while(FindNextFile(hFind, &FindFileData) );
		FindClose(hFind);
	}
	hFind = FindFirstFile("CLEO\\CLEO_PLUGINS\\*.cleo", &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char filename[MAX_PATH];
				strcpy(filename, "CLEO\\CLEO_PLUGINS\\");
				strcat(filename, FindFileData.cFileName);
				CleoPlugin *plugin = new CleoPlugin(FindFileData.cFileName, filename);
				if(plugin)
				{
					if(!plugin->module)
					{
						delete plugin;
						LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Failed to load plugin %s", plugin->name);
					}
					else
					{
						plugin->next = plugins;
						plugins = plugin;
						numLoadedPlugins++;
						LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Loaded plugin %s", plugin->name);
					}
				}
				else
					LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Failed to load plugin %s", FindFileData.cFileName);
			}
		}
		while(FindNextFile(hFind, &FindFileData) );
		FindClose(hFind);
	}
}

void CleoPlugins::UnloadPlugins()
{
	CleoPlugin *plugin = plugins;
	while(plugin)
	{
		numLoadedPlugins--;
		CleoPlugin *next = plugin->next;
		LOGL(LOG_PRIORITY_SCRIPT_LOADING, "Unloaded plugin %s", plugin->name);
		delete plugin;
		plugin = next;
	}
	plugins = 0;
}