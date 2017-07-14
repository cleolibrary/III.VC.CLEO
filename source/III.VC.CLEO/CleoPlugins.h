#pragma once
#include <Windows.h>

class CleoPlugin
{
public:

	char *name;

	HMODULE module;

	CleoPlugin *next;

	CleoPlugin(char *pluginname, char *librarypath);

	~CleoPlugin();
};

class CleoPlugins
{
public:

	static unsigned int numLoadedPlugins;

	static void LoadPlugins();

	static void UnloadPlugins();

	static CleoPlugin *plugins;
};