#include "Fxt.h"
#include <stdio.h>
#include <Windows.h>
#include "Log.h"
#include "Game.h"

CustomTextEntry *CustomText::pCustomTextList;

CustomTextEntry::CustomTextEntry(char *key, char *text)
{
	unsigned int len = strlen(text);
	this->m_pText = new wchar_t[len + 1];
	for(int i = 0; i < len; i++)
		this->m_pText[i] =  (unsigned char)text[i];
	this->m_pText[len] = 0;
	strncpy(m_key, key, 7);
	this->m_key[7] = '\0';
	LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Registered custom text: \"%s\", \"%s\"", this->m_key, text);
}

CustomTextEntry::~CustomTextEntry()
{
	if(this->m_pText)
		delete m_pText;
}

wchar_t *CustomText::GetText(int theText, int, char *key)
{
	wchar_t *result = NULL;
	CustomTextEntry *entry = pCustomTextList;
	while(entry)
	{
		if(!stricmp(entry->m_key, key))
		{
			result = entry->m_pText;
			break;
		}
		entry = entry->m_pNext;
	}
	if(!result)
		result = game.Text.pfGetText(theText, key);
	if(!result)
		return L"";
	return result;
}

char *StrFindKeyBegin(char *str)
{
	while(*str && *str != '\n' && *str != '\r' && *str != ';' && *str != '#')
	{
		if(*str != ' ' && *str != '\t')
			return str;
		str++;
	}
	return NULL;
}

char *StrFindKeyEnd(char *str)
{
	while(*str && *str != '\n' && *str != '\r' && *str != ';' && *str != '#')
	{
		if(*str == ' ' || *str == '\t')
			return str;
		str++;
	}
	return NULL;
}

char *StrFindTextBegin(char *str)
{
	while(*str && *str != '\n' && *str != '\r')
	{
		if(str[0] == '\\' && str[1] == '$')
			return str + 2;
		if(*str != ' ' && *str != '\t')
			return str;
		str++;
	}
	return NULL;
}

char *StrFindTextEnd(char *str)
{
	while(*str != '\0' && *str != '\n' && *str != '\r')
		str++;
	return str;
}

void CustomText::LoadFxtFile(char *filepath)
{
	FILE *fxt = fopen(filepath, "rt");
	char line[512];
	if(fgets(line, 512, fxt))
	{
		do
		{
			char *keyBegin = StrFindKeyBegin(line);
			if(keyBegin)
			{
				char *keyEnd = StrFindKeyEnd(&keyBegin[1]);
				if(keyEnd)
				{
					*keyEnd = '\0';
					char *textBegin = StrFindTextBegin(&keyEnd[1]);
					if(textBegin)
					{
						char *textEnd = StrFindTextEnd(&textBegin[1]);
						*textEnd = '\0';
						CustomTextEntry *entry = new CustomTextEntry(keyBegin, textBegin);
						if(entry)
						{
							entry->m_pNext = CustomText::pCustomTextList;
							CustomText::pCustomTextList = entry;
						}
					}
				}
			}
		}
		while(fgets(line, 512, fxt));
	}
}

void CustomText::Load()
{
	WIN32_FIND_DATA FindFileData;
	memset(&FindFileData, 0, sizeof(WIN32_FIND_DATA));
	HANDLE hFind = FindFirstFile("CLEO\\CLEO_TEXT\\*.fxt", &FindFileData);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				char filename[MAX_PATH];
				strcpy(filename, "CLEO\\CLEO_TEXT\\");
				strcat(filename, FindFileData.cFileName);
				LoadFxtFile(filename);
			}
		}
		while(FindNextFile(hFind, &FindFileData) );
		FindClose(hFind);
	}
}

void CustomText::Unload()
{
	CustomTextEntry *entry = pCustomTextList;
	while(entry)
	{
		LOGL(LOG_PRIORITY_CUSTOM_TEXT, "Unloaded custom text \"%s\"", entry->m_key);
		CustomTextEntry *next = entry->m_pNext;
		delete entry;
		entry = next;
	}
	pCustomTextList = 0;
}