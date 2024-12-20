#include "Fxt.h"
#include <stdio.h>
#include <Windows.h>
#include "Log.h"
#include "Game.h"

CustomTextEntry *CustomText::pCustomTextList;

CustomTextEntry::CustomTextEntry(char *key, char *text)
{
	size_t len = strlen(text);
	this->m_pText = new wchar16_t[len + 1];
	static bool is_cn = GtaGame::IsChineseVersion();
	if (is_cn)
	{
		CustomText::Utf8ToUtf16(text, this->m_pText, len, len + 1);
	}
	else
	{
		for (size_t i = 0; i < len; i++) 
			this->m_pText[i] = (unsigned char)text[i];
	}
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

void CustomText::Utf8ToUtf16(const char *utf8, wchar16_t *utf16, size_t utf8_len, size_t utf16_len)
{
	static auto get_utf8_bytes = [](uint8_t utf8) -> uint8_t
	{
			for (uint8_t i = 0; i < 6; i++)
			{
				if ((utf8 & (0x80 >> i)) == 0)
					return i == 0 ? 1 : i;
			}
			return 1;
	};
	
	size_t len = 0;
	for (size_t i = 0; i < utf8_len && len < utf16_len; i++, len++)
	{
		uint8_t bytes = get_utf8_bytes(static_cast<uint8_t>(utf8[i]));
		if (bytes > 1)
		{
			utf16[len] = utf8[i] & (0xFF >> (bytes + 1));
			for (uint8_t j = 1; j < bytes; j++)
			{
				i++;
				utf16[len] <<= 6;
				utf16[len] += utf8[i] & 0x3F;
			}
		}
		else
		{
			utf16[len] = utf8[i];
		}
	}
	utf16[len] = 0;
}

wchar_t *CustomText::GetText(int theText, int, char *key)
{
	wchar_t *result = nullptr;
	CustomTextEntry *entry = pCustomTextList;
	while(entry)
	{
		if(!_stricmp(entry->m_key, key))
		{
			result = reinterpret_cast<wchar_t*>(entry->m_pText);
			break;
		}
		entry = entry->m_pNext;
	}
	if(!result)
		result = game.Text.pfGetText(theText, key);
	if(!result)
		return nullptr;
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
	return nullptr;
}

char *StrFindKeyEnd(char *str)
{
	while(*str && *str != '\n' && *str != '\r' && *str != ';' && *str != '#')
	{
		if(*str == ' ' || *str == '\t')
			return str;
		str++;
	}
	return nullptr;
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
	return nullptr;
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